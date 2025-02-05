#include "Control.h"
#include "Cleanup.h"

using namespace std;

Control::Control() : Wnd()
{
}

Control::Control(HWND hwnd) : Wnd(hwnd)
{
}

int Control::ID() const
{
    return GetDlgCtrlID(this->hwnd);
}

void Control::Subclass(const WNDFUNC& proc)
{
    const static auto wndproc = (WNDPROC)[](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)->LRESULT
    {
        auto data = (Subdata*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
        ONCLEANUP(data, [=]
        {
            if (WM_NCDESTROY == uMsg)
            {
                SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)data->defproc);
                SetWindowLongPtrW(hWnd, GWLP_USERDATA, 0);
                delete (Subdata*)data;
            }
        });
        return data->subproc(hWnd, uMsg, wParam, lParam);
    };

    if (!this->hwnd)
    {
        return;
    }

    auto subdata = (Subdata*)GetWindowLongPtrW(this->hwnd, GWLP_USERDATA);
    auto defproc = subdata ? subdata->defproc : nullptr;

    delete subdata;
    SetWindowLongPtrW(this->hwnd, GWLP_USERDATA, 0);

    if (!proc)
    {
        if (defproc)
        {
            SetWindowLongPtrW(this->hwnd, GWLP_WNDPROC, (LONG_PTR)defproc);
        }

        return;
    }

    subdata = new Subdata();
    subdata->defproc = defproc ? defproc : (WNDPROC)GetWindowLongPtrW(this->hwnd, GWLP_WNDPROC);
    subdata->subproc = proc;
    SetWindowLongPtrW(this->hwnd, GWLP_USERDATA, (LONG_PTR)subdata);
    SetWindowLongPtrW(this->hwnd, GWLP_WNDPROC,  (LONG_PTR)wndproc);
}

LRESULT Control::DefWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    auto defproc = ((Subdata*)GetWindowLongPtrW(hWnd, GWLP_USERDATA))->defproc;
    return CallWindowProcW(defproc, hWnd, uMsg, wParam, lParam);
}