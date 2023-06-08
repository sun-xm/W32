#include "Dialog.h"

using namespace std;

Dialog::Dialog(UINT dialogId) : id(dialogId)
{
}

Dialog::~Dialog()
{
}

bool Dialog::Create(HWND parent, HINSTANCE instance)
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

Control Dialog::Item(int dlgItemId) const
{
    return Control(GetDlgItem(this->hwnd, dlgItemId));
}

bool Dialog::OnCreated()
{
    return true;
}

void Dialog::OnDestroy()
{
}

bool Dialog::OnCommand()
{
    return false;
}

void Dialog::OnClose()
{
    auto modal = this->StyleEx() & WS_EX_DLGMODALFRAME;
    this->Destroy();

    if (modal)
    {
        PostQuitMessage(0);
    }
}

void Dialog::OnSize()
{
}

void Dialog::RegisterMessage(UINT message, const function<Result()>& handler)
{
    this->messages[message] = make_pair(false, handler);
}

void Dialog::RegisterCommand(WORD command, const function<bool()>& handler)
{
    this->commands[command] = make_pair(false, handler);
}

void Dialog::RemoveMessage(UINT message)
{
    auto it = this->messages.find(message);
    if (it != this->messages.end())
    {
        this->messages.erase(it);
    }
}

void Dialog::RemoveCommand(WORD command)
{
    auto it = this->commands.find(command);
    if (it != this->commands.end())
    {
        this->commands.erase(it);
    }
}

void Dialog::SetResult(LONG_PTR result)
{
    SetWindowLongPtrW(this->hwnd, DWLP_MSGRESULT, result);
}

BOOL Dialog::DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    this->message = uMsg;
    this->wparam = wParam;
    this->lparam = lParam;

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

Dialog::Result Dialog::Handled(bool handled, LONG_PTR result)
{
    return Result(handled, result);
}

INT_PTR Dialog::MessageRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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