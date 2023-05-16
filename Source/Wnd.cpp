#include "Wnd.h"
#include <vector>

using namespace std;

Wnd::Wnd() : hwnd(0)
{
}

Wnd::Wnd(HWND hWnd) : hwnd(hWnd)
{
}

Wnd Wnd::Parent() const
{
    return Wnd(GetParent(this->hwnd));
}

void Wnd::Parent(const Wnd& parent)
{
    SetParent(this->hwnd, parent);
}

bool Wnd::Post(UINT uMsg, WPARAM wParam, LPARAM lParam) const
{
    return PostMessageW(this->hwnd, uMsg, wParam, lParam) ? true : false;
}

LRESULT Wnd::Send(UINT uMsg, WPARAM wParam, LPARAM lParam) const
{
    return SendMessageW(this->hwnd, uMsg, wParam, lParam);
}

RECT Wnd::ClientRect() const
{
    RECT rect = {0};
    GetClientRect(this->hwnd, &rect);
    return rect;
}

RECT Wnd::WindowRect() const
{
    RECT rect = {0};
    GetWindowRect(this->hwnd, &rect);
    return rect;
}

void Wnd::Show()
{
    ShowWindow(this->hwnd, SW_SHOW);
}

void Wnd::Hide()
{
    ShowWindow(this->hwnd, SW_HIDE);
}

void Wnd::Update() const
{
    UpdateWindow(this->hwnd);
}

bool Wnd::IsVisible() const
{
    return IsWindowVisible(this->hwnd) ? true : false;
}

void Wnd::Enable()
{
    EnableWindow(this->hwnd, TRUE);
}

void Wnd::Disable()
{
    EnableWindow(this->hwnd, FALSE);
}

bool Wnd::IsEnabled() const
{
    return IsWindowEnabled(this->hwnd) ? true : false;
}

bool Wnd::HasFocus() const
{
    if (this->hwnd && this->hwnd == GetFocus())
    {
        return true;
    }

    return false;
}

void Wnd::SetFocus()
{
    ::SetFocus(this->hwnd);
}

int Wnd::X() const
{
    auto rect = this->WindowRect();
    auto parent = this->Parent();

    if (parent)
    {
        POINT client;
        ClientToScreen(parent, &client);
        rect.left -= client.x;
    }

    return rect.left;
}

int Wnd::Y() const
{
    auto rect = this->WindowRect();
    auto parent = this->Parent();

    if (parent)
    {
        POINT client;
        ClientToScreen(parent, &client);
        rect.top -= client.y;
    }

    return rect.top;
}

int Wnd::Width() const
{
    auto rect = this->WindowRect();
    return rect.right - rect.left;
}

int Wnd::Height() const
{
    auto rect = this->WindowRect();
    return rect.bottom - rect.top;
}

int Wnd::ClientW() const
{
    auto rect = this->ClientRect();
    return rect.right - rect.left;
}

int Wnd::ClientH() const
{
    auto rect = this->ClientRect();
    return rect.bottom - rect.top;
}

void Wnd::SetFont(const Font& font)
{
    this->Send(WM_SETFONT, (WPARAM)(HFONT)font, TRUE);
}

void Wnd::Text(const wstring& text)
{
    SetWindowTextW(this->hwnd, text.c_str());
}

wstring Wnd::Text() const
{
    wstring text;

    auto len = GetWindowTextLengthW(this->hwnd);
    if (!len)
    {
        return text;
    }

    text.resize(len + 1);
    text.resize(GetWindowTextW(this->hwnd, &text[0], (int)text.length()));

    return text;
}

DWORD Wnd::Style() const
{
    return (DWORD)GetWindowLongPtrW(this->hwnd, GWL_STYLE);
}

DWORD Wnd::StyleEx() const
{
    return (DWORD)GetWindowLongPtrW(this->hwnd, GWL_EXSTYLE);
}

void Wnd::Style(DWORD style)
{
    SetWindowLongPtrW(this->hwnd, GWL_STYLE, style);
    SetWindowPos(this->hwnd, 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_DRAWFRAME);
}

void Wnd::StyleEx(DWORD style)
{
    SetWindowLongPtrW(this->hwnd, GWL_EXSTYLE, style);
    SetWindowPos(this->hwnd, 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_DRAWFRAME);
}

void Wnd::ClipChildren(bool clip)
{
    if (clip)
    {
        this->Style(this->Style() | WS_CLIPCHILDREN);
    }
    else
    {
        this->Style(this->Style() &~ WS_CLIPCHILDREN);
    }
}

void Wnd::ClipSiblings(bool clip)
{
    if (clip)
    {
        this->Style(this->Style() | WS_CLIPSIBLINGS);
    }
    else
    {
        this->Style(this->Style() &~ WS_CLIPSIBLINGS);
    }
}

void Wnd::Destroy()
{
    DestroyWindow(this->hwnd);
    this->hwnd = 0;
}

Wnd::operator HWND() const
{
    return this->hwnd;
}

Wnd::operator bool() const
{
    return this->hwnd ? true : false;
}