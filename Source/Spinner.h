#pragma once

#include "Control.h"

class Spinner : public Control
{
public:
    Spinner();
    Spinner(HWND);

    void Buddy(HWND);
    HWND Buddy() const;

    void Position(int);
    bool Position(int&) const;

    void Range(int  min, int  max);
    void Range(int& min, int& max);
};