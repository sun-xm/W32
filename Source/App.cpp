#include "App.h"
#include "MainWindow.h"

App* App::instance = nullptr;

App::App(HINSTANCE instance, UINT accelerator) : hinst(instance), accel(accelerator)
{
    App::instance = this;
}

App::operator HINSTANCE()
{
    return this->hinst;
}

int App::Run(MainWindow& main, int nCmdShow)
{
    if (!main.Create())
    {
        return -1;
    }

    ShowWindow(main, nCmdShow);

    return MessageLoop(main, this->accel ? LoadAcceleratorsW(this->hinst, MAKEINTRESOURCEW(this->accel)) : nullptr);
}

App& App::Instance()
{
    return *instance;
}

int App::MessageLoop(HWND hwnd, HACCEL haccel)
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
            if (haccel && TranslateAcceleratorW(hwnd, haccel, &msg))
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