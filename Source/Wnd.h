#pragma once

#include <string>
#include <Windows.h>

class Wnd
{
public:
    Wnd() : hwnd(0) {}
    Wnd(HWND hWnd) : hwnd(hWnd) {}

    Wnd Owner() const
    {
        if (!this->hwnd)
        {
            return Wnd();
        }

        return Wnd((HWND)GetWindowLongPtrW(this->hwnd, GWLP_HWNDPARENT));
    }
    void Owner(HWND owner)
    {
        SetWindowLongPtrW(this->hwnd, GWLP_HWNDPARENT, (LONG_PTR)owner);
        SetWindowPos(this->hwnd, 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_DRAWFRAME);
    }

    Wnd Parent() const { return Wnd(GetParent(this->hwnd)); }
    void Parent(HWND parent) { SetParent(this->hwnd, parent); }

    bool Post(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0) const { return PostMessageW(this->hwnd, uMsg, wParam, lParam) ? true : false; }
    LRESULT Send(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0) const { return SendMessageW(this->hwnd, uMsg, wParam, lParam); }

    RECT ClientRect() const
    {
        RECT rect = {0};
        GetClientRect(this->hwnd, &rect);
        return rect;
    }
    RECT WindowRect() const
    {
        RECT rect = {0};
        GetWindowRect(this->hwnd, &rect);
        return rect;
    }

    void Show() { ShowWindow(this->hwnd, SW_SHOW); }
    void Hide() { ShowWindow(this->hwnd, SW_HIDE); }
    void Update() const { UpdateWindow(this->hwnd); }
    bool IsVisible() const { return IsWindowVisible(this->hwnd) ? true : false; }

    void Invalidate(bool erase = false) const { InvalidateRect(this->hwnd, nullptr, erase ? TRUE : FALSE); }
    void Invalidate(const RECT& rect, bool erase = false) const { InvalidateRect(this->hwnd, &rect,  erase ? TRUE : FALSE); }

    void Enable() { EnableWindow(this->hwnd, TRUE); }
    void Disable() { EnableWindow(this->hwnd, FALSE); }
    bool IsEnabled() const { return IsWindowEnabled(this->hwnd) ? true : false; }

    bool HasFocus() const
    {
        if (this->hwnd && this->hwnd == GetFocus())
        {
            return true;
        }

        return false;
    }
    void SetFocus() { ::SetFocus(this->hwnd); }

    POINT Position() const
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

    int X() const { return this->Position().x; }
    int Y() const { return this->Position().y; }
    int Width() const
    {
        auto rect = this->WindowRect();
        return rect.right - rect.left;
    }
    int Height() const
    {
        auto rect = this->WindowRect();
        return rect.bottom - rect.top;
    }
    int ClientW() const
    {
        auto rect = this->ClientRect();
        return rect.right - rect.left;
    }
    int ClientH() const
    {
        auto rect = this->ClientRect();
        return rect.bottom - rect.top;
    }

    void MoveTo(int x, int y, bool repaint = false)
    {
        auto rect = this->WindowRect();
        MoveWindow(this->hwnd, x, y, rect.right - rect.left, rect.bottom - rect.top, repaint ? TRUE : FALSE);
    }
    void Resize(int w, int h, bool repaint = true)
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
    void ResizeClient(int w, int h, bool repaint = true)
    {
        auto wrect = this->WindowRect();
        auto crect = this->ClientRect();

        auto ww = wrect.right - wrect.left;
        auto wh = wrect.bottom - wrect.top;

        auto cw = crect.right - crect.left;
        auto ch = crect.bottom - crect.top;

        this->Resize(w + ww - cw, h + wh - ch, repaint);
    }

    void SetFont(HFONT font) { this->Send(WM_SETFONT, (WPARAM)font, TRUE); }
    void SetMenu(HMENU menu) { ::SetMenu(this->hwnd, menu); }

    void Text(const std::wstring& text) { SetWindowTextW(this->hwnd, text.c_str()); }
    std::wstring Text() const
    {
        std::wstring text;

        auto len = GetWindowTextLengthW(this->hwnd);
        if (!len)
        {
            return text;
        }

        text.resize(len + 1);
        text.resize(GetWindowTextW(this->hwnd, &text[0], (int)text.length()));

        return text;
    }

    DWORD Style() const { return (DWORD)GetWindowLongPtrW(this->hwnd, GWL_STYLE); }
    DWORD StyleEx() const { return (DWORD)GetWindowLongPtrW(this->hwnd, GWL_EXSTYLE); }
    void  Style(DWORD style)
    {
        SetWindowLongPtrW(this->hwnd, GWL_STYLE, style);
        SetWindowPos(this->hwnd, 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_DRAWFRAME);
    }
    void  StyleEx(DWORD style)
    {
        SetWindowLongPtrW(this->hwnd, GWL_EXSTYLE, style);
        SetWindowPos(this->hwnd, 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_DRAWFRAME);
    }

    bool   SetProp(const wchar_t* name, HANDLE data) { return ::SetPropW(this->hwnd, name, data) ? true : false; }
    HANDLE GetProp(const wchar_t* name) const { return ::GetPropW(this->hwnd, name); }
    HANDLE RemoveProp(const wchar_t* name) { return ::RemovePropW(this->hwnd, name); }

    void ClipChildren(bool clip = true)
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
    void ClipSiblings(bool clip = true)
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

    void Close() { this->Send(WM_CLOSE); }
    void Destroy()
    {
        DestroyWindow(this->hwnd);
        this->hwnd = 0;
    }

    operator HWND() const { return this->hwnd; }
    operator bool() const { return this->hwnd ? true : false; }

protected:
    HWND hwnd;
};