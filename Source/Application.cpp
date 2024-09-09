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

int Application::Run(Dialog& dialog, int nCmdShow, bool isDialog)
{
    if (!dialog.Create(nullptr, this->hinst))
    {
        return -1;
    }

    dialog.StyleEx(dialog.StyleEx() | WS_EX_DLGMODALFRAME);
    ShowWindow(dialog, nCmdShow);

    return MessageLoop(dialog, Application::Accel(), isDialog);
}

int Application::Run(Dialog&& dialog, int nCmdShow, bool isDialog)
{
    return Run((Dialog&)dialog, nCmdShow, isDialog);
}

int Application::Run(Window& window, int nCmdShow, bool isDialog)
{
    if (!window.Create(0, WS_OVERLAPPEDWINDOW, 0, this->hinst))
    {
        return -1;
    }

    ShowWindow(window, nCmdShow);

    return MessageLoop(window, Application::Accel(), isDialog);
}

int Application::Run(Window&& window, int nCmdShow, bool isDialog)
{
    return Run((Window&)window, nCmdShow, isDialog);
}

Application& Application::Instance()
{
    return *instance;
}

HACCEL Application::Accel()
{
    return instance->accel;
}

int Application::MessageLoop(HWND hWnd, HACCEL hAcc, bool isDialog)
{
    MSG  msg = {0};
    BOOL ret;

    while (0 != (ret = GetMessageW(&msg, nullptr, 0, 0)))
    {
        if (-1 == ret)
        {
            break;
        }

        if (TranslateAcceleratorW(hWnd, hAcc, &msg))
        {
            continue;
        }

        if (isDialog && IsDialogMessageW(hWnd, &msg))
        {
            continue;
        }

        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return (int)msg.wParam;
}