#pragma once

#include "Control.h"
#include <vector>

class ListView : public Control
{
public:
    ListView() {}
    ListView(HWND hwnd) : Control(hwnd) {}

    bool Create(HWND parent, UINT id, DWORD style = LVS_ALIGNLEFT | WS_BORDER, HINSTANCE instance = nullptr)
    {
        if (!parent || this->hwnd)
        {
            return false;
        }

        style |= WS_CHILD;

        this->hwnd = CreateWindowExW(0, WC_LISTVIEWW, nullptr, style, 0, 0, 0, 0, parent, (HMENU)(UINT_PTR)id, instance, nullptr);

        return !!this->hwnd;
    }

    void  ExtendedStyle(DWORD style)
    {
        ListView_SetExtendedListViewStyle(this->hwnd, style);
    }
    DWORD ExtendedStyle() const
    {
        return ListView_GetExtendedListViewStyle(this->hwnd);
    }

    // First column cannot be other than left-aligned. An empty first column can used as walkaround
    int  AddColumn(const std::wstring& title, int width = 0, int format = LVCFMT_LEFT)
    {
        return this->InsertColumn(this->ColumnCount(), title, width, format);
    }
    int  InsertColumn(int index, const std::wstring& title, int width, int format = LVCFMT_LEFT)
    {
        LVCOLUMNW col;
        col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_TEXT;
        col.cx   = width;
        col.fmt  = format;
        col.pszText = (LPWSTR)title.c_str();
        col.iSubItem = index;

        index = ListView_InsertColumn(this->hwnd, index, &col);

        if (index >= 0)
        {
            col.mask = LVCF_SUBITEM;
            int columns = this->ColumnCount();
            for (int i = index + 1; i < columns; i++)
            {
                col.iSubItem = i;
                ListView_SetColumn(this->hwnd, i, &col);
            }
        }

        return index;
    }
    bool RemoveColumn(int index)
    {
        if (!ListView_DeleteColumn(this->hwnd, index))
        {
            return false;
        }

        LVCOLUMNW col;
        col.mask = LVCF_SUBITEM;

        auto columns = this->ColumnCount();
        for (int i = index; i < columns; i++)
        {
            col.iSubItem = i;
            ListView_SetColumn(this->hwnd, i, &col);
        }

        return true;
    }

    int  ColumnCount() const
    {
        auto header = ListView_GetHeader(this->hwnd);
        return Header_GetItemCount(header);
    }
    int  ColumnWidth(int index) const
    {
        return ListView_GetColumnWidth(this->hwnd, index);
    }
    bool ColumnWidth(int index, int width)
    {
        return ListView_SetColumnWidth(this->hwnd, index, width) ? true : false;
    }

    int  Add(const std::wstring& text, void* data = nullptr)
    {
        return this->Insert(this->Count(), text, data);
    }
    int  Insert(int index, const std::wstring& text, void* data = nullptr)
    {
        LVITEMW item;
        item.mask = LVIF_TEXT | LVIF_PARAM;
        item.iItem = index;
        item.lParam = (LPARAM)data;
        item.pszText = (LPWSTR)text.c_str();
        item.iSubItem = 0;
        return ListView_InsertItem(this->hwnd, &item);
    }
    bool Remove(int index)
    {
        return ListView_DeleteItem(this->hwnd, index) ? true : false;
    }
    int  Count() const
    {
        return ListView_GetItemCount(this->hwnd);
    }
    void Clear()
    {
        ListView_DeleteAllItems(this->hwnd);
    }

    bool  Data(int index, void* data)
    {
        LVITEMW item;
        item.mask = LVIF_PARAM;
        item.iItem = index;
        item.iSubItem = 0;
        return ListView_SetItem(this->hwnd, &item) ? true : false;
    }
    void* Data(int index) const
    {
        LVITEMW item;
        item.mask = LVIF_PARAM;
        item.iItem = index;
        item.iSubItem = 0;
        return ListView_GetItem(this->hwnd, &item) ? (void*)item.lParam : nullptr;
    }
    void* Data() const
    {
        auto index = this->Selection();
        return index < 0 ? nullptr : this->Data(index);
    }

    bool  Text(int index, int column, const std::wstring& text)
    {
        LVITEMW item;
        item.mask = LVIF_TEXT;
        item.iItem =  index;
        item.pszText = (LPWSTR)text.c_str();
        item.iSubItem = column;
        return ListView_SetItem(this->hwnd, &item) ? true : false;
    }
    std::wstring Text(int index, int column = 0) const
    {
        LVITEMW item;
        item.mask = LVIF_TEXT;
        item.iItem =  index;
        item.iSubItem = column;
        return ListView_GetItem(this->hwnd, &item) ? std::wstring(item.pszText) : std::wstring();
    }
    std::wstring Text() const
    {
        auto index = this->Selection();
        return index < 0 ? std::wstring() : this->Text(index, 0);
    }

    int Selection() const
    {
        auto count = this->Count();
        for (int i = 0; i < count; i++)
        {
            if (ListView_GetItemState(this->hwnd, i, LVIS_SELECTED) & LVIS_SELECTED)
            {
                return i;
            }
        }

        return -1;
    }
    std::vector<int> Selections() const
    {
        std::vector<int> selections;
        auto count = this->Count();
        for (int i = 0; i < count; i++)
        {
            if (ListView_GetItemState(this->hwnd, i, LVIS_SELECTED) & LVIS_SELECTED)
            {
                selections.push_back(i);
            }
        }
        return selections;
    }

    bool Select(int index)  // To highlight selected item, need to set focus to ListView control
    {
        LVITEMW item;
        item.stateMask = LVIS_SELECTED;
        item.state = LVIS_SELECTED;
        return this->Send(LVM_SETITEMSTATE, (WPARAM)index, (LPARAM)&item) ? true : false;
    }
    bool Deselect(int index)
    {
        LVITEMW item;
        item.stateMask = LVIS_SELECTED;
        item.state = 0;
        return this->Send(LVM_SETITEMSTATE, (WPARAM)index, (LPARAM)&item) ? true : false;
    }
    void ClearSelection()
    {
        this->Deselect(-1);
    }
};