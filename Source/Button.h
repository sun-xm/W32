#pragma once

#include "Control.h"

class Button : public Control
{
public:
    bool Create(HWND parent, UINT id, const std::wstring& text, DWORD style = 0, HINSTANCE instance = nullptr)
    {
        if (!parent || this->hwnd)
        {
            return false;
        }

        style |= WS_CHILD;

        this->hwnd = CreateWindowExW(0, WC_BUTTONW, text.c_str(), style, 0, 0, 0, 0, parent, (HMENU)(UINT_PTR)id, instance, nullptr);

        return !!this->hwnd;
    }
};