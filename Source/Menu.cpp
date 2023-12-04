#include "Menu.h"

MenuItem::MenuItem() : menu(nullptr), id(0)
{
}

MenuItem::MenuItem(HMENU menu, UINT id) : menu(menu), id(id)
{
}

void MenuItem::Check(bool checked)
{
    CheckMenuItem(this->menu, this->id, MF_BYCOMMAND | (checked ? MF_CHECKED : MF_UNCHECKED));
}

void MenuItem::Uncheck()
{
    this->Check(false);
}

void MenuItem::Enable(bool enabled)
{
    EnableMenuItem(this->menu, this->id, MF_BYCOMMAND | (enabled ? MF_ENABLED : MF_GRAYED));
}

void MenuItem::Disable()
{
    this->Enable(false);
}

bool MenuItem::IsChecked() const
{
    auto state = GetMenuState(this->menu, this->id, MF_BYCOMMAND);
    if (-1 == state)
    {
        return false;
    }

    return (state & MF_CHECKED) ? true : false;
}

bool MenuItem::IsEnabled() const
{
    auto state = GetMenuState(this->menu, this->id, MF_BYCOMMAND);
    if (-1 == state)
    {
        return false;
    }

    return (state & MF_ENABLED) ? true : false;
}

MenuItem::operator bool() const
{
    return !!this->menu;
}


RadioMenu::RadioMenu() : menu(nullptr), first(0), last(0)
{
}

RadioMenu::RadioMenu(HMENU menu, UINT first, UINT last) : menu(menu), first(first), last(last)
{
}

void RadioMenu::Check(UINT id, bool checked)
{
    CheckMenuRadioItem(this->menu, this->first, this->last, checked ? id : 0, MF_BYCOMMAND);
}

void RadioMenu::Uncheck(UINT id)
{
    this->Check(id, false);
}

UINT RadioMenu::GetChecked() const
{
    for (UINT id = this->first; id <= this->last; id++)
    {
        auto state = GetMenuState(this->menu, id, MF_BYCOMMAND);

        if (-1 != state && state & MF_CHECKED)
        {
            return id;
        }
    }

    return 0;
}


Menu::Menu() : menu(nullptr), parent(nullptr), pos(0)
{
}

Menu::Menu(HMENU menu, HMENU parent, int pos) : menu(menu), parent(parent), pos(pos)
{
}

Menu::~Menu()
{
    this->Destroy();
}

bool Menu::Create(UINT id, HINSTANCE instance)
{
    this->Destroy();
    this->menu = LoadMenuW(instance, MAKEINTRESOURCEW(id));
    this->parent = nullptr;
    this->pos = 0;
    return !!this->menu;
}

void Menu::Destroy()
{
    if (!this->parent)
    {
        DestroyMenu(this->menu);
        this->menu = nullptr;
        this->pos = 0;
    }
}

void Menu::Enable()
{
    EnableMenuItem(this->parent, this->pos, MF_BYPOSITION | MF_ENABLED);
}

void Menu::Disable()
{
    EnableMenuItem(this->parent, this->pos, MF_BYPOSITION | MF_GRAYED);
}

void Menu::Popup(HWND hwnd, int x, int y, UINT flags)
{
    TrackPopupMenu(this->menu, flags, x, y, 0, hwnd, nullptr);
}

Menu Menu::SubMenuContains(UINT id) const
{
    HMENU parent;
    int pos;
    auto menu = Search(this->parent, this->menu, id, 0, parent, pos);
    return Menu(menu, parent, pos);
}

Menu Menu::SubMenuByPos(int pos) const
{
    auto item = GetSubMenu(*this, pos);
    return Menu(item, this->menu, pos);
}

RadioMenu Menu::RadioMenu(UINT first, UINT last) const
{
    HMENU parent;
    int pos;
    auto menu = Search(this->parent, this->menu, first, 0, parent, pos);
    return ::RadioMenu(menu, first, last);
}

MenuItem Menu::Item(UINT id) const
{
    HMENU parent;
    int pos;
    return MenuItem(Search(this->parent, this->menu, id, 0, parent, pos), id);
}

HMENU Menu::Search(HMENU parent, HMENU menu, UINT id, int pos, HMENU& mparent, int& mpos)
{
    auto count = GetMenuItemCount(menu);
    for (int i = 0; i < count; i++)
    {
        auto subm = GetSubMenu(menu, i);
        if (subm)
        {
            auto m = Search(menu, subm, id, i, mparent, mpos);
            if (m)
            {
                return m;
            }
        }
        else
        {
            if (id == GetMenuItemID(menu, i))
            {
                mparent = parent;
                mpos = pos;
                return menu;
            }
        }
    }

    return nullptr;
}

Menu::operator HMENU() const
{
    return this->menu;
}

Menu::operator bool() const
{
    return !!this->menu;
}