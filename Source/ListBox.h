#pragma once

#include "Control.h"

class ListBox : public Control
{
public:
    ListBox() {}
    ListBox(HWND hwnd) : Control(hwnd) {}

    bool Create(HWND parent, UINT id, DWORD style = LBS_NOINTEGRALHEIGHT | WS_BORDER | WS_VSCROLL, HINSTANCE instance = nullptr)
    {
        if (!parent || this->hwnd)
        {
            return false;
        }

        style |= WS_CHILD;

        this->hwnd = CreateWindowExW(0, WC_LISTBOXW, nullptr, style, 0, 0, 0, 0, parent, (HMENU)(UINT_PTR)id, instance, nullptr);

        return !!this->hwnd;
    }

    int Add(const std::wstring& text)
    {
        return (int)this->Send(LB_ADDSTRING, 0, (LPARAM)text.c_str());
    }
    int Add(const std::wstring& text, void* data)
    {
        auto index = this->Add(text);
        this->ItemData(index, data);
        return index;
    }
    int Insert(int index, const std::wstring& text)
    {
        return (int)this->Send(LB_INSERTSTRING, index, (LPARAM)text.c_str());
    }
    int Insert(int index, const std::wstring& text, void* data)
    {
        index = this->Insert(index, text);
        this->ItemData(index, data);
        return index;
    }
    int Remove(int index)
    {
        return (int)this->Send(LB_DELETESTRING, index);
    }
    int Count() const
    {
        return (int)this->Send(LB_GETCOUNT);
    }
    void Clear()
    {
        this->Send(LB_RESETCONTENT);
    }

    void  ItemData(int index, void* data)
    {
        this->Send(LB_SETITEMDATA, index, (LPARAM)data);
    }
    void* ItemData(int index) const
    {
        return (void*)this->Send(LB_GETITEMDATA, index);
    }

    int  TextLength(int index) const
    {
        return (int)this->Send(LB_GETTEXTLEN, index);
    }
    bool GetText(int index, std::wstring& text) const
    {
        text.clear();

        auto cnt = this->TextLength(index);
        if (LB_ERR == cnt)
        {
            return false;
        }

        text.resize(cnt + 1);
        if (LB_ERR == this->Send(LB_GETTEXT, index, (LPARAM)&text[0]))
        {
            text.clear();
            return false;
        }

        text.resize(cnt);
        return true;
    }

    std::wstring Text() const
    {
        std::wstring text;

        auto index = this->Selection();
        if (index >= 0)
        {
            this->GetText(index, text);
        }

        return text;
    }
    void* Data() const
    {
        auto index = this->Selection();
        if (index < 0)
        {
            return nullptr;
        }

        return this->ItemData(index);
    }

    int  Selection() const
    {
        return (int)this->Send(LB_GETCURSEL);
    }
    bool Select(int index)
    {
        if (index < 0 && index >= this->Count())
        {
            return false;
        }

        return LB_ERR != this->Send(LB_SETCURSEL, (WPARAM)index);
    }
    void ClearSelection()
    {
        this->Send(LB_SETCURSEL, (WPARAM)-1);
    }
};