#include "AppDialog.h"
#include "resource.h"

AppDialog::AppDialog() : Dialog(IDD_MAIN)
{
}

bool AppDialog::OnCreated()
{
    if (!Dialog::OnCreated())
    {
        return false;
    }

    if (!this->CreateStatus())
    {
        return false;
    }

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