#pragma once

#include "Control.h"
#include <functional>

class TreeViewItem
{
public:
    TreeViewItem() : tree(nullptr), item(nullptr) {}
    TreeViewItem(HWND tree, HTREEITEM item) : tree(tree), item(item) {}

    bool Text(const std::wstring& text)
    {
        TVITEMW tvi;
        tvi.mask = TVIF_TEXT | TVIF_HANDLE;
        tvi.hItem = this->item;
        tvi.pszText = (LPWSTR)text.c_str();
        return SendMessageW(this->tree, TVM_SETITEMW, 0, (LPARAM)&tvi) ? true : false;
    }
    std::wstring Text(size_t maxLength = 256) const
    {
        maxLength++;

        std::wstring buf;
        buf.resize(maxLength);

        TVITEMW tvi;
        tvi.mask = TVIF_TEXT | TVIF_HANDLE;
        tvi.hItem = this->item;
        tvi.pszText = &buf[0];
        tvi.cchTextMax = (int)maxLength;

        if (!SendMessageW(this->tree, TVM_GETITEMW, 0, (LPARAM)&tvi))
        {
            return std::wstring();
        }

        buf.resize(wcslen(tvi.pszText));
        return buf;
    }

    bool  Data(void* data)
    {
        TVITEMW tvi;
        tvi.mask = TVIF_PARAM  | TVIF_HANDLE;
        tvi.hItem = this->item;
        tvi.lParam = (LPARAM)data;
        return SendMessageW(this->tree, TVM_SETITEMW, 0, (LPARAM)&tvi) ? true : false;
    }
    void* Data() const
    {
        TVITEMW tvi;
        tvi.mask = TVIF_PARAM  | TVIF_HANDLE;
        tvi.hItem = this->item;
        if (!SendMessageW(this->tree, TVM_GETITEMW, 0, (LPARAM)&tvi))
        {
            return nullptr;
        }

        return (void*)tvi.lParam;
    }

    HWND Tree() const
    {
        return this->tree;
    }
    HWND Edit()
    {
        return (HWND)SendMessageW(this->tree, TVM_EDITLABEL, 0, (LPARAM)this->item);
    }

    void Collapse() const
    {
        SendMessageW(this->tree, TVM_EXPAND, TVE_COLLAPSE, (LPARAM)this->item);
    }
    void Expand() const
    {
        SendMessageW(this->tree, TVM_EXPAND, TVE_EXPAND, (LPARAM)this->item);
    }
    void Toggle() const
    {
        SendMessageW(this->tree, TVM_EXPAND, TVE_TOGGLE, (LPARAM)this->item);
    }
    void Select() const
    {
        SendMessageW(this->tree, TVM_SELECTITEM, TVGN_CARET, (LPARAM)this->item);
    }

    // EmurateChild() stops and returns true if any callback function return true.
    // Otherwise EmurateChild() return false;
    bool EnumerateChild(const std::function<bool(TreeViewItem& item)>& onItem)
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
    bool EnumerateChild(const std::function<bool(TreeViewItem& item, void* param)>& onItem, void* param)
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
                stop = onItem(item, param);
                child = stop ? nullptr : (HTREEITEM)SendMessageW(this->tree, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)child);

            } while (child);

            return stop;
        }

        return false;
    }
    bool EnumerateChild(const std::function<bool(const TreeViewItem& item)>& onItem) const
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
    bool EnumerateChild(const std::function<bool(const TreeViewItem& item, void* param)>& onItem, void* param) const
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
                stop = onItem(item, param);
                child = stop ? nullptr : (HTREEITEM)SendMessageW(this->tree, TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)child);

            } while (child);

            return stop;
        }

        return false;
    }

    TreeViewItem Add(const std::wstring& text, HTREEITEM after = TVI_LAST)
    {
        TVINSERTSTRUCTW insert = {0};
        insert.item.mask = TVIF_TEXT;
        insert.item.pszText = (LPWSTR)text.c_str();
        insert.hParent = this->item;
        insert.hInsertAfter = after;
        return TreeViewItem(this->tree, (HTREEITEM)SendMessageW(this->tree, TVM_INSERTITEMW, 0, (LPARAM)&insert));
    }
    TreeViewItem Add(const std::wstring& text, void* data, HTREEITEM after = TVI_LAST)
    {
        TVINSERTSTRUCTW insert = {0};
        insert.item.mask = TVIF_TEXT | TVIF_PARAM;
        insert.item.pszText = (LPWSTR)text.c_str();
        insert.item.lParam = (LPARAM)data;
        insert.hParent = this->item;
        insert.hInsertAfter = after;
        return TreeViewItem(this->tree, (HTREEITEM)SendMessageW(this->tree, TVM_INSERTITEMW, 0, (LPARAM)&insert));
    }
    void Remove()
    {
        SendMessageW(this->tree, TVM_DELETEITEM, 0, (LPARAM)this->item);
    }
    void Clear()
    {
        this->EnumerateChild([](TreeViewItem& item)
        {
            item.Remove();
            return false;
        });
    }

    operator HTREEITEM() const
    {
        return this->item;
    }

    operator bool() const
    {
        return this->tree && this->item;
    }

private:
    HWND      tree;
    HTREEITEM item;
};

class TreeView : public Control
{
public:
    TreeView() {}
    TreeView(HWND hWnd) : Control(hWnd) {}

    bool Create(HWND parent, UINT id, DWORD style = WS_BORDER | WS_HSCROLL | WS_VSCROLL | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT, HINSTANCE instance = nullptr)
    {
        if (!parent || this->hwnd)
        {
            return false;
        }

        style |= WS_CHILD;

        this->hwnd = CreateWindowExW(0, WC_TREEVIEWW, nullptr, style, 0, 0, 0, 0, parent, (HMENU)(UINT_PTR)id, instance, nullptr);

        return !!this->hwnd;
    }

    HWND GetEditControl() const
    {
        return (HWND)this->Send(TVM_GETEDITCONTROL);
    }
    bool EndEdit(bool cancel = false) const
    {
        return this->Send(TVM_ENDEDITLABELNOW, cancel ? TRUE : FALSE) ? true : false;
    }

    TreeViewItem Root() const
    {
        return TreeViewItem(this->hwnd, TVI_ROOT);
    }
    TreeViewItem Selection() const
    {
        return TreeViewItem(this->hwnd, (HTREEITEM)this->Send(TVM_GETNEXTITEM, TVGN_CARET, 0));
    }

    void ClearSelection()
    {
        this->Send(TVM_SELECTITEM, TVGN_CARET, 0);
    }
};