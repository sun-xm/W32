#pragma once

#define NOMINMAX
#include <Windows.h>

class MainWindow;

class App
{
public:
    App(HINSTANCE instance, UINT accelerator = 0);

    operator HINSTANCE();

    int Run(MainWindow&, int);

    static App& Instance();

protected:
    static int MessageLoop(HWND hWnd, HACCEL haccel);

    HINSTANCE hinst;
    UINT accel;

    static App* instance;
};