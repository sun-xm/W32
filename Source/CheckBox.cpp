#include "CheckBox.h"
#include <CommCtrl.h>

using namespace std;

CheckBox::CheckBox() : Control()
{
}

CheckBox::CheckBox(HWND hWnd) : Control(hWnd)
{
}

bool CheckBox::Create(HWND parent, UINT id, const wstring& text, DWORD style, HINSTANCE instance)
{
    if (!parent || this->hwnd)
    {
        return false;
    }

    if (!style)
    {
        style = BS_AUTOCHECKBOX;
    }

    style |= WS_CHILD;

    this->hwnd = CreateWindowExW(0, WC_BUTTONW, text.c_str(), style, 0, 0, 0, 0, parent, (HMENU)(UINT_PTR)id, instance, nullptr);

    return !!this->hwnd;
}

void CheckBox::Check()
{
    this->Send(BM_SETCHECK, BST_CHECKED);
}

void CheckBox::Uncheck()
{
    this->Send(BM_SETCHECK, BST_UNCHECKED);
}

void CheckBox::Indetermine()
{
    this->Send(BM_SETCHECK, BST_INDETERMINATE);
}

bool CheckBox::IsChecked() const
{
    return BST_CHECKED == this->Send(BM_GETCHECK);
}

bool CheckBox::IsUnchecked() const
{
    return BST_UNCHECKED == this->Send(BM_GETCHECK);
}

bool CheckBox::IsIndeterminate() const
{
    return BST_INDETERMINATE == this->Send(BM_GETCHECK);
}