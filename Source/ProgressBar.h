#pragma once

#include "Control.h"

class ProgressBar : public Control
{
public:
    ProgressBar();
    ProgressBar(HWND);

    bool Create(HWND parent, UINT id, DWORD style = 0, HINSTANCE instance = nullptr);

    void SetRange(int  min, int  max);
    void GetRange(int& min, int& max);

    void Position(int pos);
    int  Position() const;
};