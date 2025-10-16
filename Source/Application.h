#pragma once

#include <Windows.h>
#include <cstdint>
#include <string>
#include <vector>

class Dialog;
class Window;

class Application
{
public:
    Application(HINSTANCE instance, UINT accelerator = 0) : hinst(instance)
    {
        if (Inst())
        {
            throw std::runtime_error("Only one Application instance is allowed");
        }

        Inst() = this;
        this->accel = accelerator ? LoadAcceleratorsW(instance, MAKEINTRESOURCEW(accelerator)) : 0;
    }

    operator HINSTANCE()
    {
        return this->hinst;
    }

    template<typename T, typename std::enable_if<std::is_base_of<Dialog, T>::value, int>::type = 0>
    int Run(T&& dialog, int nCmdShow)
    {
        if (!dialog.Create(nullptr, this->hinst))
        {
            return -1;
        }

        ShowWindow(dialog, nCmdShow);
        return dialog.DoModal(this->accel);
    }
    template<typename T, typename std::enable_if<std::is_base_of<Window, T>::value, int>::type = 0>
    int Run(T&& window, int nCmdShow, bool likeDialog = false)
    {
        if (!window.Create(0, WS_OVERLAPPEDWINDOW, 0, this->hinst))
        {
            return -1;
        }

        ShowWindow(window, nCmdShow);

        return MessageLoop(window, Accel(), likeDialog);
    }

    static Application& Instance()
    {
        return *Inst();
    }

    static HACCEL Accel()
    {
        return Inst()->accel;
    }

    static int MessageLoop(HWND hWnd, HACCEL hAcc, bool likeDialog)
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

    static bool LoadRCData(const std::wstring& name, std::vector<uint8_t>& data)
    {
        return LoadRCData(Inst() ? Inst()->hinst : nullptr, name, data);
    }
    static bool LoadRCData(HMODULE module, const std::wstring& name, std::vector<uint8_t>& data)
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

protected:
    static Application*& Inst()
    {
        static Application* inst = nullptr;
        return inst;
    }

protected:
    HINSTANCE hinst;
    HACCEL accel;
};