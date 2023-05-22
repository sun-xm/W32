#include "StatusBar.h"

using namespace std;

StatusBar::StatusBar() : Control(0)
{
}

StatusBar::StatusBar(HWND hWnd) : Control(hWnd)
{
}

bool StatusBar::Create(HWND parent, UINT id, DWORD style, HINSTANCE instance)
{
    if (!parent || this->hwnd)
    {
        return false;
    }

    style |= WS_CHILD;

    this->hwnd = CreateWindowExW(0, STATUSCLASSNAMEW, nullptr, style, 0, 0, 0, 0, parent, (HMENU)(UINT_PTR)id, instance, nullptr);

    return !!this->hwnd;
}

void StatusBar::Adjust()
{
    this->Send(WM_SIZE);
}

bool StatusBar::SetParts(UINT parts, int* positions)
{
    return this->Send(SB_SETPARTS, parts, (LPARAM)positions) ? true : false;
}

UINT StatusBar::GetParts(vector<int>& positions) const
{
    positions.resize(this->GetParts());
    return (UINT)this->Send(SB_GETPARTS, (WPARAM)positions.size(), (LPARAM)&positions[0]);
}

UINT StatusBar::GetParts() const
{
    return (UINT)this->Send(SB_GETPARTS, 0, 0);
}

bool StatusBar::Text(const wstring& text, UINT part)
{
    return this->Send(SB_SETTEXTW, part, (LPARAM)text.c_str()) ? true : false;
}

wstring StatusBar::Text(UINT part) const
{
    wstring text;

    auto len = LOWORD(this->Send(SB_GETTEXTLENGTHW, part, 0));
    if (!len)
    {
        return text;
    }

    text.resize(len + 1);
    text.resize(LOWORD(this->Send(SB_GETTEXTW, part, (LPARAM)&text[0])));

    return text;
}