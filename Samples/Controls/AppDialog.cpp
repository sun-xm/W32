#include "AppDialog.h"
#include "resource.h"
#include <CheckBox.h>
#include <ComboBox.h>

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

    int positions[] = { 100, -1 };
    this->sbar.SetParts(2, positions);

    this->sbar.ClipChildren();
    this->sbar.Adjust();
    this->sbar.Show();

    return true;
}