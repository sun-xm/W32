#include "Menu.h"

Menu::Menu() : menu(nullptr), own(false)
{
}

Menu::Menu(HMENU menu) : menu(menu), own(false)
{
}

Menu::~Menu()
{
    this->Destroy();
}

bool Menu::Create(int id, HINSTANCE instance)
{
    this->Destroy();
    this->menu = LoadMenuW(instance, MAKEINTRESOURCEW(id));
    this->own  = true;
    return !!this->menu;
}

void Menu::Destroy()
{
    if (this->menu && this->own)
    {
        DestroyMenu(this->menu);
        this->menu = nullptr;
        this->own  = false;
    }
}

void Menu::Popup(HWND hwnd, int x, int y, UINT flags)
{
    TrackPopupMenu(this->menu, flags, x, y, 0, hwnd, nullptr);
}

void Menu::CheckItem(UINT id, bool checked)
{
    CheckMenuItem(this->menu, id, MF_BYCOMMAND | (checked ? MF_CHECKED : MF_UNCHECKED));
}

void Menu::EnableItem(UINT id, bool enabled)
{
    EnableMenuItem(this->menu, id, MF_BYCOMMAND | (enabled ? MF_ENABLED : MF_GRAYED));
}

bool Menu::IsItemChecked(UINT id) const
{
    auto state = GetMenuState(this->menu, id, MF_BYCOMMAND);
    if (-1 == state)
    {
        return false;
    }

    return (state & MF_CHECKED) ? true : false;
}

bool Menu::IsItemEnabled(UINT id) const
{
    auto state = GetMenuState(this->menu, id, MF_BYCOMMAND);
    if (-1 == state)
    {
        return false;
    }

    return (state & MF_ENABLED) ? true : false;
}

Menu Menu::Item(int pos) const
{
    auto item = GetSubMenu(*this, pos);
    return Menu(item);
}

Menu::operator HMENU() const
{
    return this->menu;
}