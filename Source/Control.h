#pragma once

#include "Wnd.h"

class Control : public Wnd
{
public:
    Control(HWND);
   ~Control();

    int ID() const;

    bool   SetProp(const wchar_t* name, HANDLE data);
    HANDLE GetProp(const wchar_t* name);
    HANDLE RemoveProp(const wchar_t* name);

    void Subclass();
    virtual LRESULT WindowProc(HWND, UINT, WPARAM, LPARAM);

    static WNDPROC Subclass(Control& ctrl, WNDPROC proc);
    static LRESULT DefWndProc(HWND, UINT, WPARAM, LPARAM);
};