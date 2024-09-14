#include "ListBox.h"

using namespace std;

ListBox::ListBox() : Control()
{
}

ListBox::ListBox(HWND hWnd) : Control(hWnd)
{
}

bool ListBox::Create(HWND parent, UINT id, DWORD style, HINSTANCE instance)
{
    if (!parent || this->hwnd)
    {
        return false;
    }

    style |= WS_CHILD;

    this->hwnd = CreateWindowExW(0, WC_LISTBOXW, nullptr, style, 0, 0, 0, 0, parent, (HMENU)(UINT_PTR)id, instance, nullptr);

    return !!this->hwnd;
}

int ListBox::Add(const wstring& text)
{
    return (int)this->Send(LB_ADDSTRING, 0, (LPARAM)text.c_str());
}

int ListBox::Add(const wstring& text, void* data)
{
    auto index = this->Add(text);
    this->ItemData(index, data);
    return index;
}

int ListBox::Insert(int index, const wstring& text)
{
    return (int)this->Send(LB_INSERTSTRING, index, (LPARAM)text.c_str());
}

int ListBox::Insert(int index, const wstring& text, void* data)
{
    index = this->Insert(index, text);
    this->ItemData(index, data);
    return index;
}

int ListBox::Remove(int index)
{
    return (int)this->Send(LB_DELETESTRING, index);
}

int ListBox::Count() const
{
    return (int)this->Send(LB_GETCOUNT);
}

void ListBox::Clear()
{
    this->Send(LB_RESETCONTENT);
}

void ListBox::ItemData(int index, void* data)
{
    this->Send(LB_SETITEMDATA, index, (LPARAM)data);
}

void* ListBox::ItemData(int index) const
{
    return (void*)this->Send(LB_GETITEMDATA, index);
}

int ListBox::TextLength(int index) const
{
    return (int)this->Send(LB_GETTEXTLEN, index);
}

bool ListBox::GetText(int index, wstring& text) const
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

wstring ListBox::Text() const
{
    wstring text;

    auto index = this->Selection();
    if (index >= 0)
    {
        this->GetText(index, text);
    }

    return text;
}

void* ListBox::Data() const
{
    auto index = this->Selection();
    if (index < 0)
    {
        return nullptr;
    }

    return this->ItemData(index);
}

int ListBox::Selection() const
{
    return (int)this->Send(LB_GETCURSEL);
}

bool ListBox::Select(int index)
{
    if (index < 0 && index >= this->Count())
    {
        return false;
    }

    return LB_ERR != this->Send(LB_SETCURSEL, (WPARAM)index);
}

void ListBox::ClearSelection()
{
    this->Send(LB_SETCURSEL, (WPARAM)-1);
}