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
    Control();
    Control(HWND);

    int ID() const;

    void Subclass(const WNDFUNC&);
    LRESULT DefWndProc(HWND, UINT, WPARAM, LPARAM) const;
};