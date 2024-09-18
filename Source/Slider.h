#pragma once

#include "Control.h"

class Slider : public Control
{
public:
    Slider();
    Slider(HWND);

    int  Position() const;
    void Position(int pos, bool redraw = true);

    int  Min() const;
    int  Max() const;
    void Min(int min, bool redraw = true);
    void Max(int max, bool redraw = true);
    void Range(unsigned short min, unsigned short max, bool redraw = true);

    bool Tick(int pos);
    void TickFreq(int freq);    // TBS_AUTOTICKS is needed
};