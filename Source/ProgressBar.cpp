#include "ProgressBar.h"
#include <CommCtrl.h>

ProgressBar::ProgressBar() : Control()
{
}

ProgressBar::ProgressBar(HWND hWnd) : Control(hWnd)
{
}

bool ProgressBar::Create(HWND parent, UINT id, DWORD style, HINSTANCE instance)
{
    if (!parent || this->hwnd)
    {
        return false;
    }

    if (!style)
    {
        style = TBS_HORZ;
    }

    style |= WS_CHILD;

    this->hwnd = CreateWindowExW(0, PROGRESS_CLASSW, nullptr, style, 0, 0, 0, 0, parent, (HMENU)(UINT_PTR)id, instance, nullptr);

    return !!this->hwnd;
}

void ProgressBar::SetRange(int min, int max)
{
    this->Send(PBM_SETRANGE32, (WPARAM)min, (LPARAM)max);
}

void ProgressBar::GetRange(int& min, int& max)
{
    PBRANGE range;
    this->Send(PBM_GETRANGE, 0, (LPARAM)&range);
    min = range.iLow;
    max = range.iHigh;
}

void ProgressBar::Position(int pos)
{
    this->Send(PBM_SETPOS, (WPARAM)pos);
}

int ProgressBar::Position() const
{
    return (int)this->Send(PBM_GETPOS);
}

void ProgressBar::Marquee(bool enable, UINT update)
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