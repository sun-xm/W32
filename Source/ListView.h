#pragma once

#include "Control.h"
#include <vector>

class ListView : public Control
{
public:
    ListView();
    ListView(HWND);

    bool Create(HWND parent, UINT id, DWORD style = LVS_ALIGNLEFT | WS_BORDER, HINSTANCE instance = nullptr);

    void  ExtendedStyle(DWORD style);
    DWORD ExtendedStyle() const;

    // First column cannot be other than left-aligned. An empty first column can used as walkaround
    int  AddColumn(const std::wstring& title, int width = 0, int format = LVCFMT_LEFT);
    int  InsertColumn(int index, const std::wstring& title, int width, int format = LVCFMT_LEFT);
    bool RemoveColumn(int index);

    int  ColumnCount() const;
    int  ColumnWidth(int index) const;
    bool ColumnWidth(int index, int width);

    int  Add(const std::wstring& text, void* data = nullptr);
    int  Insert(int index, const std::wstring& text, void* data = nullptr);
    bool Remove(int index);
    int  Count() const;
    void Clear();

    bool  Data(int index, void* data);
    void* Data(int index) const;
    void* Data() const;

    bool  Text(int index, int column, const std::wstring& text);
    std::wstring Text(int index, int column = 0) const;
    std::wstring Text() const;

    int Selection() const;
    std::vector<int> Selections() const;

    bool Select(int index); // To highlight selected item, need to set focus to ListView control
    bool Deselect(int index);
    void ClearSelection();
};