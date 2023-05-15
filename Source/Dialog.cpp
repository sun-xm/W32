#pragma once

#include "Dialog.h"
#include "App.h"

Dialog::Dialog(UINT dialogId) : id(dialogId), modal(false)
{
}

bool Dialog::Create(HWND parent)
{
    if (!this->hwnd)
    {
        CreateDialogParamW(App::Instance(),
                           MAKEINTRESOURCEW(this->id),
                           parent,
                           (DLGPROC)MessageRouter,
                           (LPARAM)this);

        if (this->hwnd)
        {
            if (!this->OnCreated())
            {
                this->Destroy();
            }
        }
    }

    return !!this->hwnd;
}

int Dialog::Modal()
{
    auto parent = this->Parent();
    if (parent)
    {
        parent.Disable();
    }

    this->modal = true;
    auto ret = (int)App::MessageLoop(this->hwnd);

    if (parent)
    {
        parent.Enable();
    }

    return ret;
}

Control Dialog::Item(int dlgItemId)
{
    return Control(GetDlgItem(this->hwnd, dlgItemId));
}

void Dialog::OnDestroy()
{
    if (this->modal)
    {
        PostQuitMessage(0);
        this->modal = false;
    }

    Window::OnDestroy();
}

LRESULT Dialog::MessageRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Dialog* dlg = nullptr;

    if (WM_INITDIALOG == uMsg)
    {
        dlg = (Dialog*)lParam;
        dlg->hwnd = hWnd;
        SetWindowLongPtrW(hWnd, GWLP_USERDATA, lParam);
        return TRUE;
    }
    else
    {
        dlg = (Dialog*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
    }

    return dlg ? dlg->WindowProc(hWnd, uMsg, wParam, lParam) : DefWindowProcW(hWnd, uMsg, wParam, lParam);
}