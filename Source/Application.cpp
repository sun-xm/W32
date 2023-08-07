#include "Application.h"
#include "Dialog.h"
#include "Window.h"

Application* Application::instance = nullptr;

Application::Application(HINSTANCE instance, UINT accelerator) : hinst(instance)
{
    if (Application::instance)
    {
        throw std::runtime_error("Only one Application instance is allowed");
    }

    Application::instance = this;
    this->accel = accelerator ? LoadAcceleratorsW(instance, MAKEINTRESOURCEW(accelerator)) : 0;
}

Application::operator HINSTANCE()
{
    return this->hinst;
}

int Application::Run(Dialog& dialog, int nCmdShow)
{
    if (!dialog.Create(nullptr, this->hinst))
    {
        return -1;
    }

    dialog.StyleEx(dialog.StyleEx() | WS_EX_DLGMODALFRAME);
    ShowWindow(dialog, nCmdShow);

    return Application::MessageLoop(dialog, Application::Accel());
}

int Application::Run(Window& window, int nCmdShow)
{
    if (!window.Create(0, WS_OVERLAPPEDWINDOW, 0, this->hinst))
    {
        return -1;
    }

    ShowWindow(window, nCmdShow);

    return MessageLoop(window, Application::Accel());
}

Application& Application::Instance()
{
    return *instance;
}

HACCEL Application::Accel()
{
    return instance->accel;
}

int Application::MessageLoop(HWND hWnd, HACCEL hAcc)
{
    MSG  msg = {0};
    BOOL ret;

    while (0 != (ret = GetMessageW(&msg, nullptr, 0, 0)))
    {
        if (-1 == ret)
        {
            break;
        }

        if (hWnd)
        {
            if (hAcc && TranslateAcceleratorW(hWnd, hAcc, &msg))
            {
                continue;
            }

            if (IsDialogMessageW(hWnd, &msg))
            {
                continue;
            }
        }

        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return (int)msg.wParam;
}