#pragma once

#include "Wnd.h"
#include <functional>
#include <map>

class Window : public Wnd
{
public:
    Window();
    virtual ~Window();

    virtual bool Create(HWND parent = 0);

protected:
    virtual bool OnCreated();
    virtual void OnDestroy();
    virtual void OnClose();
    virtual void OnPaint();
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
    DWORD style;
    DWORD styleEx;

    UINT message;
    WORD command;
    WPARAM wparam;
    LPARAM lparam;

    std::map<UINT, std::pair<bool, std::function<LRESULT()>>> messages;
    std::map<WORD, std::pair<bool, std::function<bool()>>>    commands;

    static bool Registered;
};