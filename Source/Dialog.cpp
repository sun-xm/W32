#include "Dialog.h"

#define PROP_MODAL  L"MODAL"

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

int Dialog::DoModal(HACCEL accelerator)
{
    this->SetProp(PROP_MODAL, (HANDLE)1);

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

bool Dialog::IsModal() const
{
    return this->GetProp(PROP_MODAL) ? true : false;
}

void Dialog::Center(HWND hwnd)
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
    if (this->IsModal())
    {
        PostQuitMessage(0);
    }
}

bool Dialog::OnCommand()
{
    return false;
}

void Dialog::OnClose()
{
    if (this->IsModal())
    {
        SetForegroundWindow(this->Parent());
    }
    this->Destroy();
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