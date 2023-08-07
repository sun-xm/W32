#pragma once

#include <Windows.h>

class Dialog;
class Window;

class Application
{
public:
    Application(HINSTANCE instance, UINT accelerator = 0);

    operator HINSTANCE();

    int Run(Dialog&, int);
    int Run(Window&, int);

    static Application& Instance();
    static HACCEL Accel();

    static int MessageLoop(HWND hWnd, HACCEL hAcc);

protected:
    HINSTANCE hinst;
    HACCEL accel;

    static Application* instance;
};