#include "ListView.h"

using namespace std;

ListView::ListView() : Control()
{
}

ListView::ListView(HWND hWnd) : Control(hWnd)
{
}

bool ListView::Create(HWND parent, UINT id, DWORD style, HINSTANCE instance)
{
    if (!parent || this->hwnd)
    {
        return false;
    }

    style |= WS_CHILD;

    this->hwnd = CreateWindowExW(0, WC_LISTVIEWW, nullptr, style, 0, 0, 0, 0, parent, (HMENU)(UINT_PTR)id, instance, nullptr);

    return !!this->hwnd;
}

void ListView::ExtendedStyle(DWORD style)
{
    ListView_SetExtendedListViewStyle(this->hwnd, style);
}

DWORD ListView::ExtendedStyle() const
{
    return ListView_GetExtendedListViewStyle(this->hwnd);
}

int ListView::AddColumn(const wstring& title, int width, int format)
{
    return this->InsertColumn(this->ColumnCount(), title, width, format);
}

int ListView::InsertColumn(int index, const wstring& title, int width, int format)
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

bool ListView::RemoveColumn(int index)
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

int ListView::ColumnCount() const
{
    auto header = ListView_GetHeader(this->hwnd);
    return Header_GetItemCount(header);
}

int ListView::ColumnWidth(int index) const
{
    return ListView_GetColumnWidth(this->hwnd, index);
}

bool ListView::ColumnWidth(int index, int width)
{
    return ListView_SetColumnWidth(this->hwnd, index, width) ? true : false;
}

int ListView::Add(const wstring& text, void* data)
{
    return this->Insert(this->Count(), text, data);
}

int ListView::Insert(int index, const wstring& text, void* data)
{
    LVITEMW item;
    item.mask = LVIF_TEXT | LVIF_PARAM;
    item.iItem = index;
    item.lParam = (LPARAM)data;
    item.pszText = (LPWSTR)text.c_str();
    item.iSubItem = 0;
    return ListView_InsertItem(this->hwnd, &item);
}

bool ListView::Remove(int index)
{
    return ListView_DeleteItem(this->hwnd, index) ? true : false;
}

int ListView::Count() const
{
    return ListView_GetItemCount(this->hwnd);
}

void ListView::Clear()
{
    ListView_DeleteAllItems(this->hwnd);
}

bool ListView::Data(int index, void* data)
{
    LVITEMW item;
    item.mask = LVIF_PARAM;
    item.iItem = index;
    item.iSubItem = 0;
    return ListView_SetItem(this->hwnd, &item) ? true : false;
}

void* ListView::Data(int index) const
{
    LVITEMW item;
    item.mask = LVIF_PARAM;
    item.iItem = index;
    item.iSubItem = 0;
    return ListView_GetItem(this->hwnd, &item) ? (void*)item.lParam : nullptr;
}

void* ListView::Data() const
{
    auto index = this->Selection();
    return index < 0 ? nullptr : this->Data(index);
}

bool ListView::Text(int index, int column, const wstring& text)
{
    LVITEMW item;
    item.mask = LVIF_TEXT;
    item.iItem =  index;
    item.pszText = (LPWSTR)text.c_str();
    item.iSubItem = column;
    return ListView_SetItem(this->hwnd, &item) ? true : false;
}

wstring ListView::Text(int index, int column) const
{
    LVITEMW item;
    item.mask = LVIF_TEXT;
    item.iItem =  index;
    item.iSubItem = column;
    return ListView_GetItem(this->hwnd, &item) ? wstring(item.pszText) : wstring();
}

wstring ListView::Text() const
{
    auto index = this->Selection();
    return index < 0 ? wstring() : this->Text(index, 0);
}

int ListView::Selection() const
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

vector<int> ListView::Selections() const
{
    vector<int> selections;
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

bool ListView::Select(int index)
{
    LVITEMW item;
    item.stateMask = LVIS_SELECTED;
    item.state = LVIS_SELECTED;
    return this->Send(LVM_SETITEMSTATE, (WPARAM)index, (LPARAM)&item) ? true : false;
}

bool ListView::Deselect(int index)
{
    LVITEMW item;
    item.stateMask = LVIS_SELECTED;
    item.state = 0;
    return this->Send(LVM_SETITEMSTATE, (WPARAM)index, (LPARAM)&item) ? true : false;
}

void ListView::ClearSelection()
{
    this->Deselect(-1);
}