#include "StatusBar.h"
#include "App.h"
#include <CommCtrl.h>

using namespace std;

StatusBar::StatusBar() : Control(0)
{
}

StatusBar::StatusBar(HWND hWnd) : Control(hWnd)
{
}

bool StatusBar::Create(UINT dlgItemId, HWND parent)
{
    if (!parent || this->hwnd)
    {
        return false;
    }

    this->hwnd = CreateWindowExW(0, STATUSCLASSNAMEW, nullptr, CCS_BOTTOM | WS_CHILD, 0, 0, 0, 0, parent, (HMENU)(UINT_PTR)dlgItemId, App::Instance(), nullptr);
    auto e = GetLastError();
    return !!this->hwnd;
}

void StatusBar::Adjust()
{
    this->Send(WM_SIZE);
}

bool StatusBar::SetParts(UINT parts, int* positions)
{
    if (parts > 256 || !positions)
    {
        return false;
    }

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