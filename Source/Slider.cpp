#include "Slider.h"
#include <CommCtrl.h>

Slider::Slider() : Control()
{
}

Slider::Slider(HWND hWnd) : Control(hWnd)
{
}

int Slider::Position() const
{
    return (int)this->Send(TBM_GETPOS);
}

void Slider::Position(int pos, bool redraw)
{
    this->Send(TBM_SETPOS, redraw ? TRUE : FALSE, (LPARAM)pos);
}

int Slider::Min() const
{
    return (int)this->Send(TBM_GETRANGEMIN);
}

int Slider::Max() const
{
    return (int)this->Send(TBM_GETRANGEMAX);
}

void Slider::Min(int min, bool redraw)
{
    this->Send(TBM_SETRANGEMIN, redraw ? TRUE : FALSE, (LPARAM)min);
}

void Slider::Max(int max, bool redraw)
{
    this->Send(TBM_SETRANGEMAX, redraw ? TRUE : FALSE, (LPARAM)max);
}

void Slider::Range(unsigned short min, unsigned short max, bool redraw)
{
    this->Send(TBM_SETRANGE, redraw ? TRUE : FALSE, (LPARAM)((max << 16) | min));
}