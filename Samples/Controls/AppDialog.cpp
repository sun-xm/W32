#include "AppDialog.h"
#include "resource.h"
#include <CommCtrl.h>
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
                this->sbar.Text(combo.Text());
            }
        }

        return true;
    });

    auto pos = combo.Position();

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

            this->sbar.Text(echo.Text());
        }

        return true;
    });


    this->RegisterCommand(IDC_BUTTON, [this]
    {
        wstring text = L"Button clicked";
        this->Item(IDC_ECHO).Text(text);
        this->sbar.Text(text);
        return true;
    });

    return true;
}

void AppDialog::OnSize()
{
    this->sbar.Adjust();
    Dialog::OnSize();
}

bool AppDialog::CreateStatus()
{
    if (!this->sbar.Create(1000, this->hwnd))
    {
        return false;
    }
    Control::Subclass(this->sbar, [](HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)->LRESULT
    {
        if (WM_SIZE == umsg)
        {
            auto combo = Wnd(GetDlgItem(hwnd, IDC_STATUS_COMBO));
            combo.MoveTo(0, 2);
            combo.Resize(50, 120);

            auto pos = combo.Position();
            int a = 0;
        }

        return Control::DefWndProc(hwnd, umsg, wparam, lparam);
    });

    ComboBox combo;
    if (!combo.Create(this->sbar, IDC_STATUS_COMBO))
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

    int positions[] = { 100, -1 };
    this->sbar.SetParts(2, positions);

    this->sbar.ClipChildren();
    this->sbar.Adjust();
    this->sbar.Show();

    return true;
}