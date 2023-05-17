#pragma once

#include "Control.h"

class ProgressBar : public Control
{
public:
    bool Create(HWND parent, UINT id, HINSTANCE instance = nullptr);

    void SetRange(int  min, int  max);
    void GetRange(int& min, int& max);

    void Position(int pos);
    int  Position() const;
};