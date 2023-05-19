#pragma once

#include "Control.h"

class Spinner : public Control
{
public:
    Spinner();
    Spinner(HWND);

    void Buddy(HWND);
    HWND Buddy() const;

    void SetPos(int);
    bool GetPos(int&) const;

    void Range(int  min, int  max);
    void Range(int& min, int& max);
};