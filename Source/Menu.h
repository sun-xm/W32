#pragma once

#include "Windows.h"

class Menu
{
public:
    Menu();
    Menu(HMENU menu);
   ~Menu();

    bool Create(int id, HINSTANCE instance = nullptr);
    void Destroy();

    void Popup(HWND hwnd, int x, int y, UINT flags = TPM_RIGHTBUTTON);

    void CheckItem(UINT id, bool checked);
    void EnableItem(UINT id, bool enabled);
    bool IsItemChecked(UINT id) const;
    bool IsItemEnabled(UINT id) const;

    Menu Item(int pos) const;

    operator HMENU() const;

private:
    HMENU menu;
    bool  own;
};