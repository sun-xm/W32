#pragma once

#include "Control.h"

class Spinner : public Control
{
public:
    Spinner() {}
    Spinner(HWND hwnd) : Control(hwnd) {}

    void Buddy(HWND hWnd)
    {
        this->Send(UDM_SETBUDDY, (WPARAM)hWnd);
    }
    HWND Buddy() const
    {
        return (HWND)this->Send(UDM_GETBUDDY);
    }

    void SetPos(int pos)
    {
        this->Send(UDM_SETPOS32, 0, (LPARAM)pos);
    }
    bool GetPos(int& pos) const
    {
        BOOL success;
        pos = (int)this->Send(UDM_GETPOS32, 0, (LPARAM)&success);
        return success ? false : true;
    }

    void Range(int  min, int  max)
    {
        this->Send(UDM_SETRANGE32, (WPARAM)min, (LPARAM)max);
    }
    void Range(int& min, int& max)
    {
        this->Send(UDM_GETRANGE32, (WPARAM)&min, (LPARAM)&max);
    }
};