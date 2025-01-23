#include "Control.h"
#include "Cleanup.h"

using namespace std;
using WndProcFn = function<LRESULT(HWND, UINT, WPARAM, LPARAM)>;

Control::Control() : Wnd(), defProc(0)
{
}

Control::Control(HWND hwnd) : Wnd(hwnd), defProc(0)
{
}

int Control::ID() const
{
    return GetDlgCtrlID(this->hwnd);
}

void Control::Subclass(const WndProcFn& proc)
{
    const static auto wndproc = (WNDPROC)[](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)->LRESULT
    {
        auto func = GetWindowLongPtrW(hWnd, GWLP_USERDATA);
        ONCLEANUP(func, [=]
        {
            if (WM_NCDESTROY == uMsg)
            {
                delete (WndProcFn*)func;
            }
        });
        return (*(WndProcFn*)func)(hWnd, uMsg, wParam, lParam);
    };

    if (!this->hwnd)
    {
        return;
    }

    if (this->defProc)
    {
        delete (WndProcFn*)GetWindowLongPtrW(this->hwnd, GWLP_USERDATA);
        SetWindowLongPtrW(this->hwnd, GWLP_USERDATA, 0);

        if (!proc)
        {
            SetWindowLongPtrW(this->hwnd, GWLP_WNDPROC, (LONG_PTR)this->defProc);
            this->defProc = nullptr;
            return;
        }
    }
    else
    {
        this->defProc = (WNDPROC)GetWindowLongPtrW(this->hwnd, GWLP_WNDPROC);
        SetWindowLongPtrW(this->hwnd, GWLP_WNDPROC, (LONG_PTR)wndproc);
    }

    SetWindowLongPtrW(this->hwnd, GWLP_USERDATA, (LONG_PTR)new WndProcFn(proc));
}

LRESULT Control::DefWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return CallWindowProcW(this->defProc, hWnd, uMsg, wParam, lParam);
}