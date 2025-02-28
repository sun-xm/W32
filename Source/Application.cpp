#include "Application.h"
#include "Dialog.h"
#include "Window.h"
#include <stdexcept>

using namespace std;

Application* Application::instance = nullptr;

Application::Application(HINSTANCE instance, UINT accelerator) : hinst(instance)
{
    if (Application::instance)
    {
        throw std::runtime_error("Only one Application instance is allowed");
    }

    Application::instance = this;
    this->accel = accelerator ? LoadAcceleratorsW(instance, MAKEINTRESOURCEW(accelerator)) : 0;
}

Application::operator HINSTANCE()
{
    return this->hinst;
}

int Application::Run(Dialog& dialog, int nCmdShow)
{
    if (!dialog.Create(nullptr, this->hinst))
    {
        return -1;
    }

    ShowWindow(dialog, nCmdShow);
    return dialog.DoModal(this->accel);
}

int Application::Run(Dialog&& dialog, int nCmdShow)
{
    return Run((Dialog&)dialog, nCmdShow);
}

int Application::Run(Window& window, int nCmdShow, bool likeDialog)
{
    if (!window.Create(0, WS_OVERLAPPEDWINDOW, 0, this->hinst))
    {
        return -1;
    }

    ShowWindow(window, nCmdShow);

    return MessageLoop(window, Application::Accel(), likeDialog);
}

int Application::Run(Window&& window, int nCmdShow, bool likeDialog)
{
    return Run(window, nCmdShow, likeDialog);
}

Application& Application::Instance()
{
    return *instance;
}

HACCEL Application::Accel()
{
    return instance->accel;
}

int Application::MessageLoop(HWND hWnd, HACCEL hAcc, bool likeDialog)
{
    MSG  msg = {0};
    BOOL ret;

    while (0 != (ret = GetMessageW(&msg, nullptr, 0, 0)))
    {
        if (-1 == ret)
        {
            break;
        }

        if (TranslateAcceleratorW(hWnd, hAcc, &msg))
        {
            continue;
        }

        if (likeDialog && IsDialogMessageW(hWnd, &msg))
        {
            continue;
        }

        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return (int)msg.wParam;
}

bool Application::LoadRCData(const wstring& name, vector<uint8_t>& data)
{
    return LoadRCData(instance ? instance->hinst : nullptr, name, data);
}

bool Application::LoadRCData(HMODULE module, const wstring& name, vector<uint8_t>& data)
{
    data.clear();

    auto res = FindResourceW(module, name.c_str(), MAKEINTRESOURCEW(10));
    if (!res)
    {
        return false;
    }

    auto size = SizeofResource(module, res);
    if (!size)
    {
        return 0 == GetLastError();
    }

    auto gmem = LoadResource(module, res);
    if (!gmem)
    {
        return false;
    }

    auto addr = LockResource(gmem);
    if (!addr)
    {
        return false;
    }

    data.assign((uint8_t*)addr, (uint8_t*)addr + size);

    return true;
}