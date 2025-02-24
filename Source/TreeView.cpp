#include "TreeView.h"
#include "Cleanup.h"

using namespace std;

TreeViewItem::TreeViewItem() : tree(nullptr), item(nullptr)
{
}

TreeViewItem::TreeViewItem(HWND tree, HTREEITEM item) : tree(tree), item(item)
{
}

bool TreeViewItem::Text(const wstring& text)
{
    TVITEMW tvi;
    tvi.mask = TVIF_TEXT | TVIF_HANDLE;
    tvi.hItem = this->item;
    tvi.pszText = (LPWSTR)text.c_str();
    return SendMessageW(this->tree, TVM_SETITEMW, 0, (LPARAM)&tvi) ? true : false;
}

wstring TreeViewItem::Text(size_t maxLength) const
{
    maxLength++;
    auto buf = new wchar_t[maxLength];
    ONCLEANUP(buf, [buf]{ delete[] buf; });

    TVITEMW tvi;
    tvi.mask = TVIF_TEXT | TVIF_HANDLE;
    tvi.hItem = this->item;
    tvi.pszText = buf;
    tvi.cchTextMax = (int)maxLength;

    if (!SendMessageW(this->tree, TVM_GETITEM, 0, (LPARAM)&tvi))
    {
        return wstring();
    }

    return wstring(buf);
}

bool TreeViewItem::Data(void* data)
{
    TVITEMW tvi;
    tvi.mask = TVIF_PARAM  | TVIF_HANDLE;
    tvi.hItem = item;
    tvi.lParam = (LPARAM)data;
    return SendMessageW(this->tree, TVM_SETITEMW, 0, (LPARAM)&tvi) ? true : false;
}

void* TreeViewItem::Data() const
{
    TVITEMW tvi;
    tvi.mask = TVIF_PARAM  | TVIF_HANDLE;
    tvi.hItem = item;
    if (!SendMessageW(this->tree, TVM_GETITEMW, 0, (LPARAM)&tvi))
    {
        return nullptr;
    }

    return (void*)tvi.lParam;
}

void TreeViewItem::Select()
{
    SendMessageW(this->tree, TVM_SELECTITEM, TVGN_CARET, (LPARAM)this->item);
}

bool TreeViewItem::EnumerateChild(const function<bool(TreeViewItem&)>& onItem)
{
    if (onItem)
    {
        auto child = (HTREEITEM)SendMessageW(this->tree, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)this->item);
        if (!child)
        {
            return false;
        }

        bool stop;
        do
        {
            auto item = TreeViewItem(this->tree, child);
            stop = onItem(item);
            child = stop ? nullptr : (HTREEITEM)SendMessageW(this->tree, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)child);

        } while (child);

        return stop;
    }

    return false;
}

bool TreeViewItem::EnumerateChild(const function<bool(const TreeViewItem&)>& onItem) const
{
    if (onItem)
    {
        auto child = (HTREEITEM)SendMessageW(this->tree, TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)this->item);
        if (!child)
        {
            return false;
        }

        bool stop;
        do
        {
            auto item = TreeViewItem(this->tree, child);
            stop = onItem(item);
            child = stop ? nullptr : (HTREEITEM)SendMessageW(this->tree, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)child);

        } while (child);

        return stop;
    }

    return false;
}

TreeViewItem TreeViewItem::Add(const std::wstring& text, HTREEITEM after)
{
    TVINSERTSTRUCTW insert = {0};
    insert.item.mask = TVIF_TEXT;
    insert.item.pszText = (LPWSTR)text.c_str();
    insert.hParent = this->item;
    insert.hInsertAfter = after;
    return TreeViewItem(this->tree, (HTREEITEM)SendMessageW(this->tree, TVM_INSERTITEMW, 0, (LPARAM)&insert));
}

TreeViewItem TreeViewItem::Add(const std::wstring& text, void* data, HTREEITEM after)
{
    TVINSERTSTRUCTW insert = {0};
    insert.item.mask = TVIF_TEXT | TVIF_PARAM;
    insert.item.pszText = (LPWSTR)text.c_str();
    insert.item.lParam = (LPARAM)data;
    insert.hParent = this->item;
    insert.hInsertAfter = after;
    return TreeViewItem(this->tree, (HTREEITEM)SendMessageW(this->tree, TVM_INSERTITEMW, 0, (LPARAM)&insert));
}

void TreeViewItem::Remove()
{
    SendMessageW(this->tree, TVM_DELETEITEM, 0, (LPARAM)this->item);
}

void TreeViewItem::Clear()
{
    this->EnumerateChild([](TreeViewItem& item)
    {
        item.Remove();
        return false;
    });
}


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

TreeViewItem TreeView::Root() const
{
    return TreeViewItem(this->hwnd, TVI_ROOT);
}

TreeViewItem TreeView::Selection() const
{
    return TreeViewItem(this->hwnd, (HTREEITEM)this->Send(TVM_GETNEXTITEM, TVGN_CARET, 0));
}

void TreeView::ClearSelection()
{
    this->Send(TVM_SELECTITEM, TVGN_CARET, 0);
}