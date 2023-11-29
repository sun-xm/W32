#pragma once

#include "Windows.h"

class MenuItem
{
public:
    MenuItem();
    MenuItem(HMENU menu, UINT id);

    void Check(bool checked = true);
    void Uncheck();

    void Enable(bool enabled = true);
    void Disable();

    bool IsChecked() const;
    bool IsEnabled() const;

    operator bool() const;

private:
    HMENU menu;
    UINT  id;
};

class Menu
{
public:
    Menu();
    Menu(HMENU menu, HMENU parent, int pos);
   ~Menu();

    bool Create(UINT id, HINSTANCE instance = nullptr);
    void Destroy();
    void Enable();
    void Disable();
    void Popup(HWND hwnd, int x, int y, UINT flags = TPM_RIGHTBUTTON);

    Menu SubMenuContains(UINT id) const;
    Menu SubMenuByPos(int pos) const;

    MenuItem Item(UINT id) const;

    operator HMENU() const;
    operator bool() const;

private:
    static HMENU Search(HMENU parent, HMENU menu, UINT id, int pos, HMENU& mparent, int& mpos);

private:
    HMENU menu;
    HMENU parent;
    int pos;
};