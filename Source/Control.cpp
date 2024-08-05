#include "Control.h"

using namespace std;

Control::Control() : Wnd()
{
}

Control::Control(HWND hwnd) : Wnd(hwnd)
{
}

Control::Control(const Control& other) : Control(other.hwnd)
{
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

void Control::Subclass(const function<LRESULT(HWND, UINT, WPARAM, LPARAM)>& subproc)
{
    if (!this->hwnd)
    {
        return;
    }

    auto proc = (WNDPROC)[](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)->LRESULT
    {
        auto func = ::GetPropW(hWnd, L"~");
        if (func)
        {
            return (*(function<LRESULT(HWND, UINT, WPARAM, LPARAM)>*)func)(hWnd, uMsg, wParam, lParam);
        }

        return Control::DefWndProc(hWnd, uMsg, wParam, lParam);
    };

    auto def = SetWindowLongPtrW(this->hwnd, GWLP_WNDPROC, (LONG_PTR)proc);
    if (!GetWindowLongPtrW(this->hwnd, GWLP_USERDATA))
    {
        SetWindowLongPtrW(this->hwnd, GWLP_USERDATA, def);
    }

    delete (function<LRESULT(HWND, UINT, WPARAM, LPARAM)>*)this->GetProp(L"~");
    this->SetProp(L"~", new function<LRESULT(HWND, UINT, WPARAM, LPARAM)>([this, subproc](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)->LRESULT
    {
        function<LRESULT(HWND, UINT, WPARAM, LPARAM)>* func = nullptr;
        if (WM_DESTROY == uMsg)
        {
            func = (decltype(func))this->RemoveProp(L"~");
        }

        LRESULT result;

        if (subproc)
        {
            result = subproc(hWnd, uMsg, wParam, lParam);
        }
        else
        {
            result = Control::DefWndProc(hWnd, uMsg, wParam, lParam);
        }

        if (WM_DESTROY == uMsg)
        {
            this->hwnd = 0;
            delete func;
        }

        return result;
    }));
}

WNDPROC Control::Subclass(Control& ctrl, WNDPROC proc)
{
    if (!ctrl || !proc)
    {
        return nullptr;
    }

    delete (function<LRESULT(HWND, UINT, WPARAM, LPARAM)>*)::RemovePropW(ctrl, L"~");

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