#pragma once

#include "Control.h"
#include <vector>

class StatusBar : public Control
{
public:
    StatusBar();
    StatusBar(HWND hWnd);

    bool Create(HWND parent, UINT id, DWORD style = CCS_BOTTOM, HINSTANCE instance = nullptr);

    void Adjust();

    bool SetParts(UINT parts, int* rightEdgePositions);
    UINT GetParts(std::vector<int>& positions) const;
    UINT GetParts() const;

    bool Text(const std::wstring& text, UINT part = 0);
    std::wstring Text(UINT part = 0) const;
};