#pragma once

#include <string>

#define NOMINMAX
#include <Windows.h>

class Wnd
{
public:
    Wnd();
    Wnd(HWND);
    virtual ~Wnd();

    Wnd Parent() const;
    void Parent(const Wnd&);

    bool Post(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0) const;
    LRESULT Send(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0) const;

    RECT ClientRect() const;
    RECT WindowRect() const;

    void Show();
    void Hide();
    void Update() const;
    bool IsVisible() const;

    void Enable();
    void Disable();
    bool IsEnabled() const;

    bool HasFocus() const;
    void SetFocus();

    int X() const;
    int Y() const;
    int Width() const;
    int Height() const;
    int ClientW() const;
    int ClientH() const;

    void Text(const std::wstring&);
    std::wstring Text() const;

    DWORD Style() const;
    DWORD StyleEx() const;
    void  Style(DWORD style);
    void  StyleEx(DWORD style);

    void ClipChildren(bool clip = true);
    void ClipSiblings(bool clip = true);

    void Destroy();

    operator HWND() const;
    operator bool() const;

protected:
    HWND hwnd;
};