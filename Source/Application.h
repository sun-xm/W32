#pragma once

#include <Windows.h>

class Dialog;
class Window;

class Application
{
public:
    Application(HINSTANCE instance, UINT accelerator = 0);

    operator HINSTANCE();

    int Run(Dialog&,  int, bool isDialog = true);
    int Run(Dialog&&, int, bool isDialog = true);
    int Run(Window&,  int, bool isDialog = false);
    int Run(Window&&, int, bool isDialog = false);

    static Application& Instance();
    static HACCEL Accel();

    static int MessageLoop(HWND hWnd, HACCEL hAcc, bool isDialog);

protected:
    HINSTANCE hinst;
    HACCEL accel;

    static Application* instance;
};