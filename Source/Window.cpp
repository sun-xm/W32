#include "Window.h"
#include "App.h"

#define WNDCLASSNAME L"{317B2C5D-2708-458B-874A-93B07E3723D1}"

using namespace std;

bool Window::Registered = false;

Window::Window() : style(0), styleEx(0)
{
}

bool Window::Create(HWND parent)
{
    if (!this->hwnd)
    {
        if (!Register())
        {
            return false;
        }

        if (parent)
        {
            this->style |= WS_CHILD;
        }
        else if (this->style)
        {
            this->style &= ~WS_CHILD;
        }
        else
        {
            this->style = WS_OVERLAPPEDWINDOW;
        }

        CreateWindowExW(this->styleEx,
                        WNDCLASSNAME,
                        L"",
                        this->style,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        parent,
                        nullptr,
                        App::Instance(),
                        this);

        if (this->hwnd)
        {
            if (!this->OnCreated())
            {
                this->Destroy();
            }
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

void Window::RegisterCommand(WORD command, const function<void()>& handler)
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

            auto it = this->commands.find(this->command);
            if (it != this->commands.end() && !it->second.first)
            {
                it->second.first = true;
                it->second.second();
                it->second.first = false;
            }
            else
            {
                auto parent = this->Parent();
                if (parent)
                {
                    parent.Send(uMsg, wParam, lParam);
                }
            }

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

    if (WM_NCCREATE == uMsg || WM_INITDIALOG == uMsg)
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