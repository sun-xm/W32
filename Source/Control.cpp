#include "Control.h"

Control::Control() : Wnd()
{
}

Control::Control(HWND hwnd) : Wnd(hwnd)
{
}

Control::~Control()
{
    this->RemoveProp(L"~");
}

int Control::ID() const
{
    return GetDlgCtrlID(this->hwnd);
}

bool Control::SetProp(const wchar_t* name, HANDLE data)
{
    return ::SetPropW(this->hwnd, name, data) ? true : false;
}

HANDLE Control::GetProp(const wchar_t* name)
{
    return ::GetPropW(this->hwnd, name);
}

HANDLE Control::RemoveProp(const wchar_t* name)
{
    return ::RemovePropW(this->hwnd, name);
}

void Control::Subclass()
{
    if (!this->hwnd)
    {
        return;
    }

    auto proc = (WNDPROC)[](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)->LRESULT
    {
        auto thiz = ::GetPropW(hWnd, L"~");
        if (thiz)
        {
            return ((Control*)thiz)->WindowProc(hWnd, uMsg, wParam, lParam);
        }

        return Control::DefWndProc(hWnd, uMsg, wParam, lParam);
    };

    auto def = SetWindowLongPtrW(this->hwnd, GWLP_WNDPROC, (LONG_PTR)proc);

    if (!GetWindowLongPtrW(this->hwnd, GWLP_USERDATA))
    {
        SetWindowLongPtrW(this->hwnd, GWLP_USERDATA, def);
    }

    this->SetProp(L"~", this);
}

LRESULT Control::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (WM_DESTROY == uMsg)
    {
        this->hwnd = 0;
    }

    return DefWndProc(hWnd, uMsg, wParam, lParam);
}

WNDPROC Control::Subclass(Control& ctrl, WNDPROC proc)
{
    if (!ctrl || !proc)
    {
        return nullptr;
    }

    auto def = SetWindowLongPtrW(ctrl, GWLP_WNDPROC, (LONG_PTR)proc);

    if (!GetWindowLongPtrW(ctrl, GWLP_USERDATA))
    {
        SetWindowLongPtrW(ctrl, GWLP_USERDATA, def);
    }

    return (WNDPROC)def;
}

LRESULT Control::DefWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return ((WNDPROC)GetWindowLongPtrW(hWnd, GWLP_USERDATA))(hWnd, uMsg, wParam, lParam);
}