#include "AppDialog.h"
#include "resource.h"
#include <ComboBox.h>

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
            }

            return true;
        }

        return false;
    });

    return true;
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