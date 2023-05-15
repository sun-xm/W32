#pragma once

#define NOMINMAX
#include <Windows.h>

class Dialog;
class MainWindow;

class App
{
public:
    App(HINSTANCE instance, UINT accelerator = 0);

    operator HINSTANCE();

    int Run(Dialog&, int);
    int Run(MainWindow&, int);

    static App& Instance();
    static HACCEL Accel();

    static int MessageLoop(HWND hWnd);

protected:
    HINSTANCE hinst;
    HACCEL accel;

    static App* instance;
};