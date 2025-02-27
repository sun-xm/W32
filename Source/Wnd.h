#pragma once

#include <string>
#include <Windows.h>

class Wnd
{
public:
    Wnd();
    Wnd(HWND);

    Wnd Owner() const;
    void Owner(HWND);

    Wnd Parent() const;
    void Parent(HWND);

    bool Post(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0) const;
    LRESULT Send(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0) const;

    RECT ClientRect() const;
    RECT WindowRect() const;

    void Show();
    void Hide();
    void Update() const;
    bool IsVisible() const;

    void Invalidate(bool erase = false) const;
    void Invalidate(const RECT& rect, bool erase = false) const;

    void Enable();
    void Disable();
    bool IsEnabled() const;

    bool HasFocus() const;
    void SetFocus();

    POINT Position() const;

    int X() const;
    int Y() const;
    int Width() const;
    int Height() const;
    int ClientW() const;
    int ClientH() const;

    void MoveTo(int x, int y, bool repaint = false);
    void Resize(int w, int h, bool repaint = true);
    void ResizeClient(int w, int h, bool repaint = true);

    void SetFont(HFONT font);
    void SetMenu(HMENU menu);

    void Text(const std::wstring&);
    std::wstring Text() const;

    DWORD Style() const;
    DWORD StyleEx() const;
    void  Style(DWORD style);
    void  StyleEx(DWORD style);

    bool   SetProp(const wchar_t* name, HANDLE data);
    HANDLE GetProp(const wchar_t* name) const;
    HANDLE RemoveProp(const wchar_t* name);

    void ClipChildren(bool clip = true);
    void ClipSiblings(bool clip = true);

    void Close();
    void Destroy();

    operator HWND() const;
    operator bool() const;

protected:
    HWND hwnd;
};