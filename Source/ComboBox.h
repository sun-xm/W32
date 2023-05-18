#pragma once

#include "Control.h"

class ComboBox : public Control
{
public:
    ComboBox();
    ComboBox(HWND);

    bool Create(HWND parent, UINT id, DWORD type = CBS_DROPDOWNLIST, HINSTANCE instance = nullptr);

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