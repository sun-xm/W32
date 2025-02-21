#pragma once

#include "Control.h"

class TreeViewItem
{
public:
    TreeViewItem(HWND tree, HTREEITEM item);

    bool Text(const std::wstring& text);
    std::wstring Text(size_t maxLength = 256) const;

    bool  Data(void* data);
    void* Data() const;

    operator HTREEITEM() const
    {
        return this->item;
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
};