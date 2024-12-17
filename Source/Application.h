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

    int Run(Dialog&,  int, bool isDialog = true);
    int Run(Dialog&&, int, bool isDialog = true);
    int Run(Window&,  int, bool isDialog = false);
    int Run(Window&&, int, bool isDialog = false);

    static Application& Instance();
    static HACCEL Accel();

    static int MessageLoop(HWND hWnd, HACCEL hAcc, bool isDialog);

    static bool LoadRCData(const std::wstring& name, std::vector<uint8_t>& data);
    static bool LoadRCData(HMODULE modle, const std::wstring& name, std::vector<uint8_t>& data);

protected:
    HINSTANCE hinst;
    HACCEL accel;

    static Application* instance;
};