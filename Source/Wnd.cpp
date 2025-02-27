#include "Wnd.h"
#include <vector>

using namespace std;

Wnd::Wnd() : hwnd(0)
{
}

Wnd::Wnd(HWND hWnd) : hwnd(hWnd)
{
}

Wnd Wnd::Owner() const
{
    if (!this->hwnd)
    {
        return Wnd();
    }

    return Wnd((HWND)GetWindowLongPtrW(this->hwnd, GWLP_HWNDPARENT));
}

void Wnd::Owner(HWND owner)
{
    SetWindowLongPtrW(this->hwnd, GWLP_HWNDPARENT, (LONG_PTR)owner);
    SetWindowPos(this->hwnd, 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_DRAWFRAME);
}

Wnd Wnd::Parent() const
{
    return Wnd(GetParent(this->hwnd));
}

void Wnd::Parent(HWND parent)
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

void Wnd::Invalidate(bool erase) const
{
    InvalidateRect(this->hwnd, nullptr, erase ? TRUE : FALSE);
}

void Wnd::Invalidate(const RECT& rect, bool erase) const
{
    InvalidateRect(this->hwnd, &rect,  erase ? TRUE : FALSE);
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

POINT Wnd::Position() const
{
    auto rect = this->WindowRect();
    POINT pos = { rect.left, rect.top };

    auto parent = this->Parent();
    if (parent)
    {
        POINT client = {0}; // Must be cleared before ClientToScreen()
        ClientToScreen(parent, &client);

        pos.x -= client.x;
        pos.y -= client.y;
    }

    return pos;
}

int Wnd::X() const
{
    return this->Position().x;
}

int Wnd::Y() const
{
    return this->Position().y;
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

void Wnd::MoveTo(int x, int y, bool repaint)
{
    auto rect = this->WindowRect();
    MoveWindow(this->hwnd, x, y, rect.right - rect.left, rect.bottom - rect.top, repaint ? TRUE : FALSE);
}

void Wnd::Resize(int w, int h, bool repaint)
{
    auto rect = this->WindowRect();
    auto parent = this->Parent();

    if (parent)
    {
        POINT client;
        ClientToScreen(parent, &client);
        rect.left -= client.x;
        rect.top  -= client.y;
    }
    MoveWindow(this->hwnd, this->X(), this->Y(), w, h, repaint ? TRUE : FALSE);
}

void Wnd::ResizeClient(int w, int h, bool repaint)
{
    auto wrect = this->WindowRect();
    auto crect = this->ClientRect();

    auto ww = wrect.right - wrect.left;
    auto wh = wrect.bottom - wrect.top;

    auto cw = crect.right - crect.left;
    auto ch = crect.bottom - crect.top;

    this->Resize(w + ww - cw, h + wh - ch, repaint);
}

void Wnd::SetFont(HFONT font)
{
    this->Send(WM_SETFONT, (WPARAM)font, TRUE);
}

void Wnd::SetMenu(HMENU menu)
{
    ::SetMenu(this->hwnd, menu);
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

bool Wnd::SetProp(const wchar_t* name, HANDLE data)
{
    return ::SetPropW(this->hwnd, name, data) ? true : false;
}

HANDLE Wnd::GetProp(const wchar_t* name) const
{
    return ::GetPropW(this->hwnd, name);
}

HANDLE Wnd::RemoveProp(const wchar_t* name)
{
    return ::RemovePropW(this->hwnd, name);
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

void Wnd::Close()
{
    this->Send(WM_CLOSE);
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