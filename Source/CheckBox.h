#pragma once

#include "Control.h"

class CheckBox : public Control
{
public:
    bool Create(HWND parent, UINT id, const std::wstring& text, DWORD type = BS_AUTOCHECKBOX, HINSTANCE instance = nullptr);

    void Check();
    void Uncheck();
    void Indetermine();

    bool IsChecked() const;
    bool IsUnchecked() const;
    bool IsIndeterminate() const;
};