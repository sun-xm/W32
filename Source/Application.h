#pragma once

#include <Windows.h>
#include <string>
#include <vector>

class Dialog;
class Window;

class Application
{
public:
    Application(HINSTANCE instance, UINT accelerator = 0);

    operator HINSTANCE();

    int Run(Dialog&,  int);
    int Run(Dialog&&, int);
    int Run(Window&,  int, bool likeDialog = false);
    int Run(Window&&, int, bool likeDialog = false);

    static Application& Instance();
    static HACCEL Accel();

    static int MessageLoop(HWND hWnd, HACCEL hAcc, bool likeDialog);

    static bool LoadRCData(const std::wstring& name, std::vector<uint8_t>& data);
    static bool LoadRCData(HMODULE modle, const std::wstring& name, std::vector<uint8_t>& data);

protected:
    HINSTANCE hinst;
    HACCEL accel;

    static Application* instance;
};