#pragma once

#include "Wnd.h"
#include <CommCtrl.h>
#include <functional>

class Control : public Wnd
{
public:
    Control();
    Control(HWND);
    Control(const Control&);

    int ID() const;

    bool   SetProp(const wchar_t* name, HANDLE data);
    HANDLE GetProp(const wchar_t* name);
    HANDLE RemoveProp(const wchar_t* name);

    // Caution: Control needs to be sustainable object to subclass
    void Subclass(const std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>&);

    static WNDPROC Subclass(Control& ctrl, WNDPROC proc);
    static LRESULT DefWndProc(HWND, UINT, WPARAM, LPARAM);
};