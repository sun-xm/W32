#include "ComboBox.h"
#include <CommCtrl.h>

using namespace std;

ComboBox::ComboBox()
{
}

ComboBox::ComboBox(HWND hWnd) : Control(hWnd)
{
}

bool ComboBox::Create(HWND parent, UINT id, DWORD style, HINSTANCE instance)
{
    if (!parent || this->hwnd)
    {
        return false;
    }

    if (!style)
    {
        style = CBS_DROPDOWNLIST;
    }

    style |= WS_CHILD | WS_VSCROLL;
    if (CBS_SIMPLE != (style & 0xF))
    {
        style |= WS_OVERLAPPED;
    }

    this->hwnd = CreateWindowExW(0, WC_COMBOBOXW, nullptr, style, 0, 0, 0, 0, parent, (HMENU)(UINT_PTR)id, instance, nullptr);

    return !!this->hwnd;
}

int ComboBox::Add(const wstring& text)
{
    return (int)this->Send(CB_ADDSTRING, 0, (LPARAM)text.c_str());
}

int ComboBox::Add(const wstring& text, void* data)
{
    auto index = this->Add(text);
    this->ItemData(index, data);
    return index;
}

int ComboBox::Insert(int index, const wstring& text)
{
    return (int)this->Send(CB_INSERTSTRING, index, (LPARAM)text.c_str());
}

int ComboBox::Insert(int index, const wstring& text, void* data)
{
    index = this->Insert(index, text);
    this->ItemData(index, data);
    return index;
}

int ComboBox::Remove(int index)
{
    return (int)this->Send(CB_DELETESTRING, index);
}

int ComboBox::Count() const
{
    return (int)this->Send(CB_GETCOUNT);
}

void ComboBox::Clear()
{
    this->Send(CB_RESETCONTENT);
}

void ComboBox::ItemData(int index, void* data)
{
    this->Send(CB_SETITEMDATA, index, (LPARAM)data);
}

void* ComboBox::ItemData(int index) const
{
    return (void*)this->Send(CB_GETITEMDATA, index);
}

int ComboBox::TextLength(int index) const
{
    return (int)this->Send(CB_GETLBTEXTLEN, index);
}

bool ComboBox::GetText(int index, wstring& text) const
{
    text.clear();

    auto cnt = this->TextLength(index);
    if (CB_ERR == cnt)
    {
        return false;
    }

    text.resize(cnt + 1);
    if (CB_ERR == this->Send(CB_GETLBTEXT, index, (LPARAM)&text[0]))
    {
        text.clear();
        return false;
    }

    text.resize(cnt);
    return true;
}

wstring ComboBox::Text() const
{
    wstring text;

    auto index = this->Selection();
    if (index >= 0)
    {
        this->GetText(index, text);
    }

    return text;
}

void* ComboBox::Data() const
{
    auto index = this->Selection();
    if (index < 0)
    {
        return nullptr;
    }

    return this->ItemData(index);
}

int ComboBox::Selection() const
{
    return (int)this->Send(CB_GETCURSEL);
}

bool ComboBox::Select(int index)
{
    if (index < 0 && index >= this->Count())
    {
        return false;
    }

    return CB_ERR != this->Send(CB_SETCURSEL, (WPARAM)index);
}

void ComboBox::ClearSelection()
{
    this->Send(CB_SETCURSEL, (WPARAM)-1);
}