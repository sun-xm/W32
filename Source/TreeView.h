#pragma once

#include "Control.h"
#include <functional>

class TreeViewItem
{
public:
    TreeViewItem();
    TreeViewItem(HWND tree, HTREEITEM item);

    bool Text(const std::wstring& text);
    std::wstring Text(size_t maxLength = 256) const;

    bool  Data(void* data);
    void* Data() const;

    void Select();

    bool EnumerateChild(const std::function<bool(TreeViewItem& item)>&); // Return true if item found in callback function then enumeration stops
                                                                         // EnumerateChild() returns true if any callback returns true
    bool EnumerateChild(const std::function<bool(const TreeViewItem& item)>&) const;

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
    TreeView();
    TreeView(HWND);

    bool Create(HWND parent, UINT id, DWORD style = WS_BORDER | WS_HSCROLL | WS_VSCROLL | TVS_HASBUTTONS | TVS_LINESATROOT, HINSTANCE instance = nullptr);

    TreeViewItem Add(const std::wstring& text, HTREEITEM parent = TVI_ROOT, HTREEITEM after = TVI_LAST);
    TreeViewItem Add(const std::wstring& text, void* data, HTREEITEM parent = TVI_ROOT, HTREEITEM after = TVI_LAST);
    void Remove(HTREEITEM item);
    void Clear();

    TreeViewItem Root() const;
    TreeViewItem Selection() const;

    void ClearSelection();
};