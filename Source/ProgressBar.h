#pragma once

#include "Control.h"

class ProgressBar : public Control
{
public:
    ProgressBar();
    ProgressBar(HWND);

    bool Create(HWND parent, UINT id, DWORD style = TBS_HORZ, HINSTANCE instance = nullptr);

    void SetRange(int  min, int  max);
    void GetRange(int& min, int& max);

    void Position(int pos);
    int  Position() const;

    void Marquee(bool enable, UINT update = 0);
};