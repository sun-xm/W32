#pragma once

#include "Control.h"

class TreeView : public Control
{
public:
    TreeView();
    TreeView(HWND);

    bool Create(HWND parent, UINT id, DWORD style = WS_BORDER | WS_HSCROLL | WS_VSCROLL | TVS_HASBUTTONS | TVS_LINESATROOT, HINSTANCE instance = nullptr);

    HTREEITEM Add(const std::wstring& text, HTREEITEM parent = TVI_ROOT, HTREEITEM after = TVI_LAST);
    HTREEITEM Add(const std::wstring& text, void* data, HTREEITEM parent = TVI_ROOT, HTREEITEM after = TVI_LAST);
    void Remove(HTREEITEM item);
    void Clear();

    bool  Data(HTREEITEM item, void* data);
    void* Data(HTREEITEM item) const;
};