#pragma once

#include "Control.h"

class ProgressBar : public Control
{
public:
    ProgressBar() {}
    ProgressBar(HWND hwnd) : Control(hwnd) {}

    bool Create(HWND parent, UINT id, DWORD style = TBS_HORZ, HINSTANCE instance = nullptr)
    {
        if (!parent || this->hwnd)
        {
            return false;
        }

        style |= WS_CHILD;

        this->hwnd = CreateWindowExW(0, PROGRESS_CLASSW, nullptr, style, 0, 0, 0, 0, parent, (HMENU)(UINT_PTR)id, instance, nullptr);

        return !!this->hwnd;
    }

    void SetRange(int  min, int  max)
    {
        this->Send(PBM_SETRANGE32, (WPARAM)min, (LPARAM)max);
    }
    void GetRange(int& min, int& max)
    {
        PBRANGE range;
        this->Send(PBM_GETRANGE, 0, (LPARAM)&range);
        min = range.iLow;
        max = range.iHigh;
    }

    void Position(int pos)
    {
        this->Send(PBM_SETPOS, (WPARAM)pos);
    }
    int  Position() const
    {
        return (int)this->Send(PBM_GETPOS);
    }

    void Marquee(bool enable, UINT update = 0)
    {
        if (enable)
        {
            this->Style(this->Style() | PBS_MARQUEE);
        }
        else
        {
            this->Style(this->Style() &~ PBS_MARQUEE);
        }

        this->Send(PBM_SETMARQUEE, enable ? TRUE : FALSE, update);
    }
};