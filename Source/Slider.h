#pragma once

#include "Control.h"

class Slider : public Control
{
public:
    Slider() {}
    Slider(HWND hwnd) : Control(hwnd) {}

    int  Position() const
    {
        return (int)this->Send(TBM_GETPOS);
    }
    void Position(int pos, bool redraw = true)
    {
        this->Send(TBM_SETPOS, redraw ? TRUE : FALSE, (LPARAM)pos);
    }

    int  Min() const
    {
        return (int)this->Send(TBM_GETRANGEMIN);
    }
    int  Max() const
    {
        return (int)this->Send(TBM_GETRANGEMAX);
    }
    void Min(int min, bool redraw = true)
    {
        this->Send(TBM_SETRANGEMIN, redraw ? TRUE : FALSE, (LPARAM)min);
    }
    void Max(int max, bool redraw = true)
    {
        this->Send(TBM_SETRANGEMAX, redraw ? TRUE : FALSE, (LPARAM)max);
    }
    void Range(unsigned short min, unsigned short max, bool redraw = true)
    {
        this->Send(TBM_SETRANGE, redraw ? TRUE : FALSE, (LPARAM)((max << 16) | min));
    }

    bool Tick(int pos)
    {
        return this->Send(TBM_SETTIC, 0, (LPARAM)pos) ? true : false;
    }
    void TickFreq(int freq) // TBS_AUTOTICKS is needed
    {
        this->Send(TBM_SETTICFREQ, (WPARAM)freq, 0);
    }
};