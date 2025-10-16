#pragma once

#include "Control.h"

class CheckBox : public Control
{
public:
    CheckBox() {}
    CheckBox(HWND hwnd) : Control(hwnd) {}

    bool Create(HWND parent, UINT id, const std::wstring& text, DWORD style = BS_AUTOCHECKBOX, HINSTANCE instance = nullptr)
    {
        if (!parent || this->hwnd)
        {
            return false;
        }

        style |= WS_CHILD;

        this->hwnd = CreateWindowExW(0, WC_BUTTONW, text.c_str(), style, 0, 0, 0, 0, parent, (HMENU)(UINT_PTR)id, instance, nullptr);

        return !!this->hwnd;
    }

    void Check()
    {
        this->Send(BM_SETCHECK, BST_CHECKED);
    }
    void Uncheck()
    {
        this->Send(BM_SETCHECK, BST_UNCHECKED);
    }
    void Indetermine()
    {
        this->Send(BM_SETCHECK, BST_INDETERMINATE);
    }

    bool IsChecked() const
    {
        return BST_CHECKED == this->Send(BM_GETCHECK);
    }
    bool IsUnchecked() const
    {
        return BST_UNCHECKED == this->Send(BM_GETCHECK);
    }
    bool IsIndeterminate() const
    {
        return BST_INDETERMINATE == this->Send(BM_GETCHECK);
    }
};