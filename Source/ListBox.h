#pragma once

#include "Control.h"

class ListBox : public Control
{
public:
    ListBox();
    ListBox(HWND);

    bool Create(HWND parent, UINT id, DWORD style = LBS_NOINTEGRALHEIGHT | WS_BORDER | WS_VSCROLL, HINSTANCE instance = nullptr);

    int Add(const std::wstring& text);
    int Add(const std::wstring& text, void* data);
    int Insert(int index, const std::wstring& text);
    int Insert(int index, const std::wstring& text, void* data);
    int Remove(int index);
    int Count() const;
    void Clear();

    void  ItemData(int index, void* data);
    void* ItemData(int index) const;

    int  TextLength(int index) const;
    bool GetText(int index, std::wstring& text) const;

    std::wstring Text() const;
    void* Data() const;

    int  Selection() const;
    bool Select(int index);
    void ClearSelection();
};