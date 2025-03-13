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

    HWND Tree() const;
    HWND Edit();

    void Collapse() const;
    void Expand() const;
    void Toggle() const;
    void Select() const;

    // EmurateChild() stops and returns true if any callback function return true.
    // Otherwise EmurateChild() return false;
    bool EnumerateChild(const std::function<bool(TreeViewItem& item)>&);
    bool EnumerateChild(const std::function<bool(TreeViewItem& item, void* param)>&, void*);
    bool EnumerateChild(const std::function<bool(const TreeViewItem& item)>&) const;
    bool EnumerateChild(const std::function<bool(const TreeViewItem& item, void* param)>&, void*) const;

    TreeViewItem Add(const std::wstring& text, HTREEITEM after = TVI_LAST);
    TreeViewItem Add(const std::wstring& text, void* data, HTREEITEM after = TVI_LAST);
    void Remove();
    void Clear();

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

    bool Create(HWND parent, UINT id, DWORD style = WS_BORDER | WS_HSCROLL | WS_VSCROLL | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT, HINSTANCE instance = nullptr);

    HWND GetEditControl() const;
    bool EndEdit(bool cancel = false) const;

    TreeViewItem Root() const;
    TreeViewItem Selection() const;

    void ClearSelection();
};