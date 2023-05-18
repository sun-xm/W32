#pragma once

#include "Control.h"

class Button : public Control
{
public:
    bool Create(HWND parent, UINT id, const std::wstring& text, DWORD style = 0, HINSTANCE instance = nullptr);
};