#pragma once

#include "Control.h"
#include <vector>

class StatusBar : public Control
{
public:
    StatusBar() {}
    StatusBar(HWND hWnd) : Control(hWnd) {}

    bool Create(HWND parent, UINT id, DWORD style = CCS_BOTTOM, HINSTANCE instance = nullptr)
    {
        if (!parent || this->hwnd)
        {
            return false;
        }

        style |= WS_CHILD;

        this->hwnd = CreateWindowExW(0, STATUSCLASSNAMEW, nullptr, style, 0, 0, 0, 0, parent, (HMENU)(UINT_PTR)id, instance, nullptr);

        return !!this->hwnd;
    }

    void Adjust()
    {
        this->Send(WM_SIZE);
    }

    bool SetParts(UINT parts, int* rightEdgePositions)
    {
        return this->Send(SB_SETPARTS, parts, (LPARAM)rightEdgePositions) ? true : false;
    }
    UINT GetParts(std::vector<int>& positions) const
    {
        positions.resize(this->GetParts());
        return (UINT)this->Send(SB_GETPARTS, (WPARAM)positions.size(), (LPARAM)&positions[0]);
    }
    UINT GetParts() const
    {
        return (UINT)this->Send(SB_GETPARTS, 0, 0);
    }

    bool Text(const std::wstring& text, UINT part = 0)
    {
        return this->Send(SB_SETTEXTW, part, (LPARAM)text.c_str()) ? true : false;
    }
    std::wstring Text(UINT part = 0) const
    {
        std::wstring text;

        auto len = LOWORD(this->Send(SB_GETTEXTLENGTHW, part, 0));
        if (!len)
        {
            return text;
        }

        text.resize(len + 1);
        text.resize(LOWORD(this->Send(SB_GETTEXTW, part, (LPARAM)&text[0])));

        return text;
    }
};