#include "App.h"
#include "Dialog.h"
#include "Window.h"

App* App::instance = nullptr;

App::App(HINSTANCE instance, UINT accelerator) : hinst(instance)
{
    App::instance = this;
    this->accel = accelerator ? LoadAcceleratorsW(instance, MAKEINTRESOURCEW(accelerator)) : 0;
}

App::operator HINSTANCE()
{
    return this->hinst;
}

int App::Run(Dialog& dialog, int nCmdShow)
{
    if (!dialog.Create(nullptr, this->hinst))
    {
        return -1;
    }

    dialog.StyleEx(dialog.StyleEx() | WS_EX_DLGMODALFRAME);
    ShowWindow(dialog, nCmdShow);

    return App::MessageLoop(dialog, App::Accel());
}

int App::Run(Window& window, int nCmdShow)
{
    if (!window.Create(0, WS_OVERLAPPEDWINDOW, 0, this->hinst))
    {
        return -1;
    }

    ShowWindow(window, nCmdShow);

    return MessageLoop(window, App::Accel());
}

App& App::Instance()
{
    return *instance;
}

HACCEL App::Accel()
{
    return instance->accel;
}

int App::MessageLoop(HWND hWnd, HACCEL hAcc)
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