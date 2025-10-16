#pragma once

#include "Windows.h"

class MenuItem
{
public:
    MenuItem() : menu(nullptr), id(0) {}
    MenuItem(HMENU menu, UINT id) : menu(menu), id(id) {}

    void Check(bool checked = true)
    {
        CheckMenuItem(this->menu, this->id, MF_BYCOMMAND | (checked ? MF_CHECKED : MF_UNCHECKED));
    }
    void Uncheck()
    {
        this->Check(false);
    }

    void Enable(bool enabled = true)
    {
        EnableMenuItem(this->menu, this->id, MF_BYCOMMAND | (enabled ? MF_ENABLED : MF_GRAYED));
    }
    void Disable()
    {
        this->Enable(false);
    }

    bool IsChecked() const
    {
        auto state = GetMenuState(this->menu, this->id, MF_BYCOMMAND);
        if (-1 == state)
        {
            return false;
        }

        return (state & MF_CHECKED) ? true : false;
    }
    bool IsEnabled() const
    {
        auto state = GetMenuState(this->menu, this->id, MF_BYCOMMAND);
        if (-1 == state)
        {
            return false;
        }

        return (state & MF_ENABLED) ? true : false;
    }

    operator bool() const
    {
        return !!this->menu;
    }

private:
    HMENU menu;
    UINT  id;
};

class RadioItems
{
public:
    RadioItems() : menu(nullptr), first(0), last(0) {}
    RadioItems(HMENU menu, UINT first, UINT last) : menu(menu), first(first), last(last) {}

    void Check(UINT id, bool checked = true)
    {
        CheckMenuRadioItem(this->menu, this->first, this->last, checked ? id : 0, MF_BYCOMMAND);
    }
    void Uncheck(UINT id)
    {
        this->Check(id, false);
    }

    UINT GetChecked() const
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

private:
    HMENU menu;
    UINT  first;
    UINT  last;
};

class Menu
{
public:
    Menu() : menu(nullptr), parent(nullptr), pos(0) {}
    Menu(HMENU menu, HMENU parent, int pos) : menu(menu), parent(parent), pos(pos) {}
   ~Menu() { this->Destroy(); }

    bool Create(UINT id, HINSTANCE instance = nullptr)
    {
        this->Destroy();
        this->menu = LoadMenuW(instance, MAKEINTRESOURCEW(id));
        this->parent = nullptr;
        this->pos = 0;
        return !!this->menu;
    }
    void Destroy()
    {
        if (!this->parent)
        {
            DestroyMenu(this->menu);
            this->menu = nullptr;
            this->pos = 0;
        }
    }
    void Enable()
    {
        EnableMenuItem(this->parent, this->pos, MF_BYPOSITION | MF_ENABLED);
    }
    void Disable()
    {
        EnableMenuItem(this->parent, this->pos, MF_BYPOSITION | MF_GRAYED);
    }
    void Popup(HWND hwnd, int x, int y, UINT flags = TPM_RIGHTBUTTON)
    {
        TrackPopupMenu(this->menu, flags, x, y, 0, hwnd, nullptr);
    }

    Menu SubMenuContains(UINT id) const
    {
        HMENU parent;
        int pos;
        auto menu = Search(this->parent, this->menu, id, 0, parent, pos);
        return Menu(menu, parent, pos);
    }
    Menu SubMenuByPos(int pos) const
    {
        auto item = GetSubMenu(*this, pos);
        return Menu(item, this->menu, pos);
    }

    RadioItems RadioItems(UINT first, UINT last) const
    {
        HMENU parent;
        int pos;
        auto menu = Search(this->parent, this->menu, first, 0, parent, pos);
        return ::RadioItems(menu, first, last);
    }

    MenuItem Item(UINT id) const
    {
        HMENU parent;
        int pos;
        return MenuItem(Search(this->parent, this->menu, id, 0, parent, pos), id);
    }

    operator HMENU() const
    {
        return this->menu;
    }
    operator bool() const
    {
        return !!this->menu;
    }

private:
    static HMENU Search(HMENU parent, HMENU menu, UINT id, int pos, HMENU& mparent, int& mpos)
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

private:
    HMENU menu;
    HMENU parent;
    int pos;
};