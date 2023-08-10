#pragma once

#include "Wnd.h"
#include <functional>
#include <map>

class Window : public Wnd
{
public:
    Window();
    virtual ~Window();

    virtual bool Create(HWND parent = 0, DWORD style = 0, DWORD styleEx = 0, HINSTANCE instance = 0);

protected:
    virtual bool OnCreated();
    virtual void OnDestroy();
    virtual bool OnCommand();
    virtual void OnClose();
    virtual void OnPaint();
    virtual void OnMove();
    virtual void OnSize();

    void RegisterMessage(UINT message, const std::function<LRESULT()>& handler);
    void RegisterCommand(WORD command, const std::function<bool()>& handler);
    void RemoveMessage(UINT message);
    void RemoveCommand(WORD command);

    virtual LRESULT WindowProc(HWND, UINT, WPARAM, LPARAM);
    virtual LRESULT DefWndProc(HWND, UINT, WPARAM, LPARAM);

    static bool Register();
    static LRESULT CALLBACK MessageRouter(HWND, UINT, WPARAM, LPARAM);

protected:
    UINT message;
    WORD command;
    WPARAM wparam;
    LPARAM lparam;

    std::map<UINT, std::pair<bool, std::function<LRESULT()>>> messages;
    std::map<WORD, std::pair<bool, std::function<bool()>>>    commands;

    static bool Registered;
};