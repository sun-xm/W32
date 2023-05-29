#include "Window.h"

#define WNDCLASSNAME L"{317B2C5D-2708-458B-874A-93B07E3723D1}"

using namespace std;

bool Window::Registered = false;

Window::Window()
{
}

Window::~Window()
{
}

bool Window::Create(HWND parent, DWORD style, DWORD styleEx, HINSTANCE instance)
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
                    WNDCLASSNAME,
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

bool Window::OnCreated()
{
    return true;
}

void Window::OnDestroy()
{
}

void Window::OnClose()
{
    auto owner = GetWindow(this->hwnd, GW_OWNER);

    this->Destroy();

    if (!owner)
    {
        PostQuitMessage(0);
    }
}

void Window::OnPaint()
{
    PAINTSTRUCT ps;
    auto hdc = BeginPaint(this->hwnd, &ps);
    EndPaint(this->hwnd, &ps);
}

void Window::OnSize()
{
}

void Window::RegisterMessage(UINT message, const function<LRESULT()>& handler)
{
    this->messages[message] = make_pair(false, handler);
}

void Window::RegisterCommand(WORD command, const function<bool()>& handler)
{
    this->commands[command] = make_pair(false, handler);
}

void Window::RemoveMessage(UINT message)
{
    auto it = this->messages.find(message);
    if (it != this->messages.end())
    {
        this->messages.erase(it);
    }
}

void Window::RemoveCommand(WORD command)
{
    auto it = this->commands.find(command);
    if (it != this->commands.end())
    {
        this->commands.erase(it);
    }
}

LRESULT Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

        case WM_SIZE:
        {
            this->OnSize();
            return 0;
        }

        case WM_COMMAND:
        {
            this->command = LOWORD(wParam);

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

    return this->DefWndProc(hWnd, uMsg, wParam, lParam);
}

LRESULT Window::DefWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

bool Window::Register()
{
    if (Registered)
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
    wcex.lpszClassName = WNDCLASSNAME;

    auto atom = RegisterClassExW(&wcex);
    Registered = !!atom;

    return Registered;
}

LRESULT Window::MessageRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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