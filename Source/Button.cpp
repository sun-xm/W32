#include "Button.h"
#include <CommCtrl.h>

using namespace std;

bool Button::Create(HWND parent, UINT id, const wstring& text, HINSTANCE instance)
{
    if (!parent || this->hwnd)
    {
        return false;
    }

    this->hwnd = CreateWindowExW(0, WC_BUTTONW, text.c_str(), WS_CHILD, 0, 0, 0, 0, parent, (HMENU)(UINT_PTR)id, instance, nullptr);

    return !!this->hwnd;
}