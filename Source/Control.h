#pragma once

#include "Wnd.h"
#include <CommCtrl.h>
#include <functional>

class Control : public Wnd
{
    using WNDFUNC = std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>;

    struct Subdata
    {
        WNDFUNC subproc;
        WNDPROC defproc;
    };

public:
    Control() {}
    Control(HWND hwnd) : Wnd(hwnd) {}

    int ID() const
    {
        return GetDlgCtrlID(this->hwnd);
    }

    void Subclass(const WNDFUNC& proc)
    {
        const static auto wndproc = (WNDPROC)[](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)->LRESULT
        {
            auto subdata = (Subdata*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
            auto result  = subdata->subproc(hWnd, uMsg, wParam, lParam);

            if (WM_NCDESTROY == uMsg)
            {
                SetWindowLongPtrW(hWnd, GWLP_WNDPROC,  (LONG_PTR)subdata->defproc);
                SetWindowLongPtrW(hWnd, GWLP_USERDATA, 0);
                delete subdata;
            }

            return result;
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

    static LRESULT DefWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        auto data = (Subdata*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
        auto proc = data ? data->defproc : (WNDPROC)GetWindowLongPtrW(hWnd, GWLP_WNDPROC);
        return CallWindowProcW(proc, hWnd, uMsg, wParam, lParam);
    }
};