#include "App.h"
#include "Dialog.h"
#include "MainWindow.h"

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
    if (!dialog.Create())
    {
        return -1;
    }

    ShowWindow(dialog, nCmdShow);

    return dialog.Modal();
}

int App::Run(MainWindow& main, int nCmdShow)
{
    if (!main.Create())
    {
        return -1;
    }

    ShowWindow(main, nCmdShow);

    return MessageLoop(main);
}

App& App::Instance()
{
    return *instance;
}

HACCEL App::Accel()
{
    return instance->accel;
}

int App::MessageLoop(HWND hwnd)
{
    MSG  msg = {0};
    BOOL ret;

    while (0 != (ret = GetMessageW(&msg, nullptr, 0, 0)))
    {
        if (-1 == ret)
        {
            break;
        }

        if (hwnd)
        {
            if (Accel() && TranslateAcceleratorW(hwnd, Accel(), &msg))
            {
                continue;
            }

            if (IsDialogMessageW(hwnd, &msg))
            {
                continue;
            }
        }

        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return (int)msg.wParam;
}