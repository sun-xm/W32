#pragma once

#include "Control.h"
#include <vector>

class StatusBar : public Control
{
public:
    StatusBar();
    StatusBar(HWND hWnd);

    bool Create(UINT dlgItemId, HWND parent);

    void Adjust();

    bool SetParts(UINT parts, int* rightEdgePositions);
    UINT GetParts(std::vector<int>& positions) const;
    UINT GetParts() const;
};