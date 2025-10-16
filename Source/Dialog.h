#pragma once

#include "Wnd.h"
#include "Control.h"
#include <functional>
#include <map>

class Dialog : public Wnd
{
public:
    typedef std::pair<bool, LONG_PTR> Result;

    Dialog(UINT dialogId) : id(dialogId) {}
    virtual ~Dialog() {}

    virtual bool Create(HWND parent = 0, HINSTANCE instance = 0)
    {
        if (!this->hwnd)
        {
            CreateDialogParamW(instance,
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

    // DS_MODALFRAME | WS_CAPTION | WS_POPUP | WS_SYSMENU can be used as default style in .rc for modal dialog
    int  DoModal(HACCEL accelerator = nullptr)
    {
        this->SetProp(PropModal(), (HANDLE)1);

        auto parent = (HWND)this->Parent();
        if (parent)
        {
            EnableWindow(parent, FALSE);
        }

        MSG  msg = {0};
        BOOL ret;

        while (0 != (ret = GetMessageW(&msg, nullptr, 0, 0)))
        {
            if (-1 == ret)
            {
                break;
            }

            if (TranslateAcceleratorW(this->hwnd, accelerator, &msg))
            {
                continue;
            }

            if (IsDialogMessageW(this->hwnd, &msg))
            {
                continue;
            }

            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        if (parent)
        {
            EnableWindow(parent, TRUE);
        }

        return (int)msg.wParam;
    }
    bool IsModal() const
    {
        return this->GetProp(PropModal()) ? true : false;
    }

    void Center(HWND hwnd = nullptr)
    {
        hwnd = hwnd ? hwnd : (HWND)this->Parent();
        if (!hwnd || !this->hwnd || !this->Parent())
        {
            return;
        }

        RECT wrect, drect;
        GetWindowRect(hwnd, &wrect);
        GetWindowRect(this->hwnd, &drect);

        auto ww = wrect.right - wrect.left;
        auto wh = wrect.bottom - wrect.top;
        auto dw = drect.right - drect.left;
        auto dh = drect.bottom - drect.top;

        auto x = (ww - dw) / 2 + wrect.left;
        auto y = (wh - dh) / 2 + wrect.top;

        RECT prect = {0};
        if (!(WS_POPUP & this->Style()))
        {
            GetWindowRect(this->Parent(), &prect);
        }

        this->MoveTo(x - prect.left, y - prect.top);
    }

    Control Item(int dlgItemId) const
    {
        return Control(GetDlgItem(this->hwnd, dlgItemId));
    }

protected:
    virtual bool OnCreated()
    {
        return true;
    }
    virtual void OnDestroy()
    {
        if (this->IsModal())
        {
            PostQuitMessage(0);
        }
    }
    virtual bool OnCommand()
    {
        return false;
    }
    virtual void OnClose()
    {
        if (this->IsModal())
        {
            SetForegroundWindow(this->Parent());
        }
        this->Destroy();
    }
    virtual void OnSize()
    {
    }

    void RegisterMessage(UINT message, const std::function<Result()>& handler)
    {
        this->messages[message] = std::make_pair(false, handler);
    }
    void RegisterCommand(WORD command, const std::function<bool()>& handler)
    {
        this->commands[command] = std::make_pair(false, handler);
    }
    void RemoveMessage(UINT message)
    {
        auto it = this->messages.find(message);
        if (it != this->messages.end())
        {
            this->messages.erase(it);
        }
    }
    void RemoveCommand(WORD command)
    {
        auto it = this->commands.find(command);
        if (it != this->commands.end())
        {
            this->commands.erase(it);
        }
    }

    void SetResult(LONG_PTR result)
    {
        SetWindowLongPtrW(this->hwnd, DWLP_MSGRESULT, result);
    }

    BOOL DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        this->message = uMsg;
        this->wparam  = wParam;
        this->lparam  = lParam;

        switch (uMsg)
        {
            case WM_COMMAND:
            {
                this->command = LOWORD(wParam);
                if (this->OnCommand())
                {
                    return TRUE;
                }

                bool handled = false;

                auto it = this->commands.find(this->command);
                if (it != this->commands.end() && !it->second.first)
                {
                    it->second.first = true;
                    handled = it->second.second();
                    it->second.first = false;
                }

                if (handled)
                {
                    return TRUE;
                }
                else
                {
                    auto parent = this->Parent();
                    if (parent)
                    {
                        return (BOOL)parent.Send(uMsg, wParam, lParam);
                    }
                }

                break;
            }

            case WM_SIZE:
            {
                this->OnSize();
                return TRUE;
            }

            case WM_CLOSE:
            {
                this->OnClose();
                return TRUE;
            }

            case WM_DESTROY:
            {
                this->OnDestroy();
                return TRUE;
            }

            default:
            {
                auto it = this->messages.find(uMsg);
                if (it != this->messages.end() && !it->second.first)
                {
                    it->second.first = true;
                    auto result = it->second.second();
                    it->second.first = false;

                    if (result.first)
                    {
                        this->SetResult(result.second);
                    }

                    return result.first ? TRUE : FALSE;
                }

                break;
            }
        }

        return FALSE; // Pass unhandled message to default dialog process
    }

    static Result Handled(bool handled, LONG_PTR result = 0)
    {
        return Result(handled, result);
    }

    static INT_PTR CALLBACK MessageRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        Dialog* dlg = nullptr;

        if (WM_INITDIALOG == uMsg)
        {
            dlg = (Dialog*)lParam;
            dlg->hwnd = hWnd;
            SetWindowLongPtrW(hWnd, GWLP_USERDATA, lParam);
        }
        else
        {
            dlg = (Dialog*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
        }

        return dlg ? dlg->DialogProc(hWnd, uMsg, wParam, lParam) : FALSE;
    }

    static const wchar_t* PropModal()
    {
        return L"MODAL";
    }

protected:
    UINT id;
    UINT message;
    WORD command;
    WPARAM wparam;
    LPARAM lparam;

    std::map<UINT, std::pair<bool, std::function<Result()>>> messages;
    std::map<WORD, std::pair<bool, std::function<bool()>>>   commands;
};