#pragma once

#include "Control.h"

class Button : public Control
{
public:
    bool Create(HWND parent, UINT id, const std::wstring& text, HINSTANCE instance = nullptr);
};