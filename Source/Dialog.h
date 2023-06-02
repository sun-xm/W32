#pragma once

#include "Wnd.h"
#include "Control.h"
#include <functional>
#include <map>

class Dialog : public Wnd
{
public:
    Dialog(UINT dialogId);
    virtual ~Dialog();

    virtual bool Create(HWND parant = 0, HINSTANCE instance = 0);

    Control Item(int dlgItemId) const;

protected:
    virtual bool OnCreated();
    virtual void OnDestroy();
    virtual bool OnCommand();
    virtual void OnClose();
    virtual void OnSize();

    void RegisterMessage(UINT message, const std::function<bool()>& handler);
    void RegisterCommand(WORD command, const std::function<bool()>& handler);
    void RemoveMessage(UINT message);
    void RemoveCommand(WORD command);

    bool SetResult(LONG_PTR result);

    BOOL DialogProc(HWND, UINT, WPARAM, LPARAM);

    static INT_PTR CALLBACK MessageRouter(HWND, UINT, WPARAM, LPARAM);

protected:
    UINT id;
    UINT message;
    WORD command;
    WPARAM wparam;
    LPARAM lparam;

    std::map<UINT, std::pair<bool, std::function<bool()>>> messages;
    std::map<WORD, std::pair<bool, std::function<bool()>>> commands;
};