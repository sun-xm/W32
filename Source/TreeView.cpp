#include "TreeView.h"

using namespace std;

TreeView::TreeView() : Control()
{
}

TreeView::TreeView(HWND hWnd) : Control(hWnd)
{
}

bool TreeView::Create(HWND parent, UINT id, DWORD style, HINSTANCE instance)
{
    if (!parent || this->hwnd)
    {
        return false;
    }

    style |= WS_CHILD;

    this->hwnd = CreateWindowExW(0, WC_TREEVIEWW, nullptr, style, 0, 0, 0, 0, parent, (HMENU)(UINT_PTR)id, instance, nullptr);

    return !!this->hwnd;
}

HTREEITEM TreeView::Add(const wstring& text, HTREEITEM parent, HTREEITEM after)
{
    TVINSERTSTRUCTW insert = {0};
    insert.item.mask = TVIF_TEXT;
    insert.item.pszText = (wchar_t*)text.c_str();
    insert.item.cchTextMax = (int)text.length() + 1;
    insert.hParent = parent;
    insert.hInsertAfter = after;
    return (HTREEITEM)this->Send(TVM_INSERTITEMW, 0, (LPARAM)&insert);
}

HTREEITEM TreeView::Add(const wstring& text, void* data, HTREEITEM parent, HTREEITEM after)
{
    TVINSERTSTRUCTW insert = {0};
    insert.item.mask = TVIF_TEXT | TVIF_PARAM;
    insert.item.pszText = (wchar_t*)text.c_str();
    insert.item.cchTextMax = (int)text.length() + 1;
    insert.item.lParam = (LPARAM)data;
    insert.hParent = parent;
    insert.hInsertAfter = after;
    return (HTREEITEM)this->Send(TVM_INSERTITEMW, 0, (LPARAM)&insert);
}

void TreeView::Remove(HTREEITEM item)
{
    this->Send(TVM_DELETEITEM, 0, (LPARAM)item);
}

void TreeView::Clear()
{
    this->Remove(TVI_ROOT);
}

bool TreeView::Data(HTREEITEM item, void* data)
{
    TVITEMW tvi;
    tvi.mask = TVIF_PARAM;
    tvi.hItem = item;
    tvi.lParam = (LPARAM)data;
    return this->Send(TVM_SETITEMW, 0, (LPARAM)&tvi) ? true : false;
}

void* TreeView::Data(HTREEITEM item) const
{
    TVITEMW tvi = {0};
    tvi.mask = TVIF_PARAM;
    tvi.hItem = item;
    this->Send(TVM_GETITEMW, 0, (LPARAM)&tvi);
    return (void*)tvi.lParam;
}