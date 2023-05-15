#pragma once

#include "Window.h"

class Dialog : public Window
{
public:
    Dialog(UINT dialogId);

    bool Create(HWND parant = 0) override;

    int Modal();

protected:
    void OnDestroy() override;

    static LRESULT CALLBACK MessageRouter(HWND, UINT, WPARAM, LPARAM);

protected:
    UINT id;
    bool modal;
};