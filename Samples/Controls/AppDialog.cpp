#include "AppDialog.h"
#include "resource.h"
#include <CommCtrl.h>
#include <Button.h>
#include <CheckBox.h>
#include <ComboBox.h>
#include <ProgressBar.h>

using namespace std;

AppDialog::AppDialog() : Dialog(IDD_MAIN), font(Font::Create(L"MS Shell Dlg", 25))
{
}

bool AppDialog::OnCreated()
{
    if (!Dialog::OnCreated())
    {
        return false;
    }
    this->Text(L"Controls");

    if (!this->CreateStatus())
    {
        return false;
    }

    this->Item(IDC_ECHO).SetFont(this->font);

    auto combo = (ComboBox&)this->Item(IDC_COMBO);
    combo.Add(L"Hello");
    combo.Add(L"World");
    this->RegisterCommand(IDC_COMBO, [this]
    {
        if (CBN_SELCHANGE == HIWORD(this->wparam))
        {
            auto combo = (ComboBox&)this->Item(IDC_COMBO);
            if (combo.Selection() >= 0)
            {
                this->Item(IDC_ECHO).Text(combo.Text());
                this->status.Text(combo.Text());
            }
        }

        return true;
    });

    ((ProgressBar&)this->Item(IDC_PROGRESS)).Position(50);

    this->RegisterCommand(IDC_CHECK, [this]
    {
        if (BN_CLICKED == HIWORD(this->wparam))
        {
            auto echo = this->Item(IDC_ECHO);
            auto check = (CheckBox&)this->Item(IDC_CHECK);

            if (check.IsChecked())
            {
                echo.Text(L"Checked");
            }
            else if (check.IsUnchecked())
            {
                echo.Text(L"Unchecked");
            }
            else
            {
                echo.Text(L"Indeterminate");
            }

            this->status.Text(echo.Text());
        }

        return true;
    });


    this->RegisterCommand(IDC_BUTTON, [this]
    {
        wstring text = L"Button clicked";
        this->Item(IDC_ECHO).Text(text);
        this->status.Text(text);
        return true;
    });

    return true;
}

void AppDialog::OnSize()
{
    this->status.Adjust();
    Dialog::OnSize();
}

bool AppDialog::CreateStatus()
{
    if (!this->status.Create(1000, this->hwnd))
    {
        return false;
    }

    Control::Subclass(this->status, [](HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)->LRESULT
    {
        auto status = (StatusBar&)Control(hwnd);

        switch (umsg)
        {
            case WM_COMMAND:
            {
                return SendMessageW(status.Parent(), umsg, wparam, lparam);
            }

            case WM_SIZE:
            {
                vector<int> positions;
                status.GetParts(positions);

                auto button = Wnd(GetDlgItem(hwnd, IDC_STATUS_BUTTON));
                button.MoveTo(positions[0], 2);
                button.Resize(60, status.Height() - button.Y());

                auto combo = Wnd(GetDlgItem(hwnd, IDC_STATUS_COMBO));
                combo.MoveTo(button.X() + button.Width(), 2);
                combo.Resize(50, 120);

                break;
            }
        }

        return Control::DefWndProc(hwnd, umsg, wparam, lparam);
    });

    Button button;
    if (!button.Create(this->status, IDC_STATUS_BUTTON, L"Button"))
    {
        return false;
    }
    button.Show();

    this->RegisterCommand(IDC_STATUS_BUTTON, [this]
    {
        this->Item(IDC_ECHO).Text(L"Status button clicked");
        this->status.Text(L"Status button clicked");
        return true;
    });

    ComboBox combo;
    if (!combo.Create(this->status, IDC_STATUS_COMBO))
    {
        return false;
    }
    combo.Add(L"0",   (void*)0);
    combo.Add(L"25",  (void*)25);
    combo.Add(L"50",  (void*)50);
    combo.Add(L"75",  (void*)75);
    combo.Add(L"100", (void*)100);
    combo.Select(2);
    combo.Show();

    int positions[] = { 150, -1 };
    this->status.SetParts(2, positions);

    this->status.ClipChildren();
    this->status.Adjust();
    this->status.Show();

    return true;
}