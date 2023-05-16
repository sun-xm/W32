#pragma once

#define NOMINMAX
#include <Windows.h>

class Dialog;
class Window;

class App
{
public:
    App(HINSTANCE instance, UINT accelerator = 0);

    operator HINSTANCE();

    int Run(Dialog&, int);
    int Run(Window&, int);

    static App& Instance();
    static HACCEL Accel();

    static int MessageLoop(HWND hWnd);

protected:
    HINSTANCE hinst;
    HACCEL accel;

    static App* instance;
};