#pragma once

#include "Wnd.h"
#include <CommCtrl.h>
#include <functional>

class Control : public Wnd
{
public:
    Control();
    Control(HWND);

    int ID() const;

    void Subclass(const std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>&);
    LRESULT DefWndProc(HWND, UINT, WPARAM, LPARAM);

protected:
    WNDPROC defProc;
};