#pragma once

#include "Wnd.h"
#include <functional>
#include <map>

class Window : public Wnd
{
public:
    Window() {}
    virtual ~Window() {}

    virtual bool Create(HWND parent = 0, DWORD style = 0, DWORD styleEx = 0, HINSTANCE instance = 0)
    {
        if (this->hwnd)
        {
            return false;
        }

        if (!Register())
        {
            return false;
        }

        if (parent)
        {
            style |= WS_CHILD;
        }
        else if (style)
        {
            style &= ~WS_CHILD;
        }
        else
        {
            style = WS_OVERLAPPEDWINDOW;
        }

        CreateWindowExW(styleEx,
                        WndClassName(),
                        L"",
                        style,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        parent,
                        nullptr,
                        instance,
                        this);

        if (this->hwnd)
        {
            if (!this->OnCreated())
            {
                this->Destroy();
            }
        }

        return !!this->hwnd;
    }

protected:
    virtual bool OnCreated()
    {
        return true;
    }
    virtual void OnDestroy()
    {
        auto owner  = GetWindow(this->hwnd, GW_OWNER);
        auto parent = GetParent(this->hwnd);
        if (!owner && !parent)
        {
            PostQuitMessage(0);
        }
    }
    virtual bool OnCommand()
    {
        return false;
    }
    virtual void OnClose()
    {
        this->Destroy();
    }
    virtual void OnPaint()
    {
        PAINTSTRUCT ps;
        BeginPaint(this->hwnd, &ps);
        EndPaint(this->hwnd, &ps);
    }
    virtual void OnMove() {}
    virtual void OnSize() {}

    void RegisterMessage(UINT message, const std::function<LRESULT()>& handler) { this->messages[message] = make_pair(false, handler); }
    void RegisterCommand(WORD command, const std::function<bool()>& handler) { this->commands[command] = make_pair(false, handler); }
    void RemoveMessage(UINT message)
    {
        auto it = this->messages.find(message);
        if (it != this->messages.end())
        {
            this->messages.erase(it);
        }
    }
    void RemoveCommand(WORD command)
    {
        auto it = this->commands.find(command);
        if (it != this->commands.end())
        {
            this->commands.erase(it);
        }
    }

    virtual LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        this->message = uMsg;
        this->wparam  = wParam;
        this->lparam  = lParam;

        switch (uMsg)
        {
            case WM_PAINT:
            {
                this->OnPaint();
                return 0;
            }

            case WM_MOVE:
            {
                this->OnMove();
                return 0;
            }

            case WM_SIZE:
            {
                this->OnSize();
                return 0;
            }

            case WM_COMMAND:
            {
                this->command = LOWORD(wParam);
                if (this->OnCommand())
                {
                    return 0;
                }

                bool handled = false;

                auto it = this->commands.find(this->command);
                if (it != this->commands.end() && !it->second.first)
                {
                    it->second.first = true;
                    handled = it->second.second();
                    it->second.first = false;
                }

                if (handled)
                {
                    return 0;
                }
                else
                {
                    auto parent = this->Parent();
                    if (parent)
                    {
                        return parent.Send(uMsg, wParam, lParam);
                    }
                }

                break;
            }

            case WM_CLOSE:
            {
                this->OnClose();
                return 0;
            }

            case WM_DESTROY:
            {
                this->OnDestroy();
                return 0;
            }

            default:
            {
                auto it = this->messages.find(uMsg);
                if (it != this->messages.end() && !it->second.first)
                {
                    it->second.first = true;
                    auto result = it->second.second();
                    it->second.first = false;
                    return result;
                }
            }
        }

        return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }

    static bool  Register()
    {
        if (Registered())
        {
            return true;
        }

        WNDCLASSEXW wcex   = {0};
        wcex.cbSize        = sizeof(wcex);
        wcex.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
        wcex.lpfnWndProc   = (WNDPROC)MessageRouter;
        wcex.hInstance     = GetModuleHandleW(nullptr);
        wcex.hIcon         = nullptr;
        wcex.hIconSm       = nullptr;
        wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = nullptr;
        wcex.lpszMenuName  = nullptr;
        wcex.lpszClassName = WndClassName();

        auto atom = RegisterClassExW(&wcex);
        Registered() = !!atom;

        return Registered();
    }
    static bool& Registered()
    {
        static bool registered = false;
        return registered;
    }
    static const wchar_t* WndClassName()
    {
        static const wchar_t* wndClassName = L"{317B2C5D-2708-458B-874A-93B07E3723D1}";
        return wndClassName;
    }
    static LRESULT CALLBACK MessageRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        Window* win = nullptr;

        if (WM_NCCREATE == uMsg)
        {
            win = (Window*)((LPCREATESTRUCTW)lParam)->lpCreateParams;
            win->hwnd = hWnd;
            SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)win);
        }
        else
        {
            win = (Window*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
        }

        return win ? win->WindowProc(hWnd, uMsg, wParam, lParam) : DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }

protected:
    UINT message;
    WORD command;
    WPARAM wparam;
    LPARAM lparam;

    std::map<UINT, std::pair<bool, std::function<LRESULT()>>> messages;
    std::map<WORD, std::pair<bool, std::function<bool()>>>    commands;
};