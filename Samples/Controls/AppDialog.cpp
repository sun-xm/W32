#include "AppDialog.h"
#include "resource.h"
#include <CommCtrl.h>
#include <Button.h>
#include <CheckBox.h>
#include <ComboBox.h>
#include <ProgressBar.h>
#include <Slider.h>
#include <Spinner.h>

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

    auto spinner = (Spinner&)this->Item(IDC_SPIN);
    spinner.Buddy(this->Item(IDC_EDIT));
    spinner.Range(0, 100);
    spinner.SetPos(50);

    this->RegisterCommand(IDC_EDIT, [this]
    {
        if (EN_CHANGE == HIWORD(this->wparam))
        {
            int pos;
            if (((Spinner&)this->Item(IDC_SPIN)).GetPos(pos))
            {
                this->Position(pos);
            }
        }

        return true;
    });

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

    ((Slider&)this->Item(IDC_SLIDER)).Position(50);
    this->RegisterMessage(WM_HSCROLL, [this]
    {
        if ((LPARAM)GetDlgItem(*this, IDC_SLIDER) == this->lparam)
        {
            this->Position(((Slider&)this->Item(IDC_SLIDER)).Position());
            return true;
        }

        return false;
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
    if (!this->status.Create(this->hwnd, IDC_STATUS))
    {
        return false;
    }

    this->status.Subclass([this](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
            case WM_COMMAND:
            {
                return this->Send(uMsg, wParam, lParam);
            }

            case WM_SIZE:
            {
                vector<int> positions;
                this->status.GetParts(positions);

                auto button = Wnd(GetDlgItem(this->status, IDC_STATUS_BUTTON));
                button.MoveTo(positions[0], 2);
                button.Resize(60, this->status.Height() - button.Y());

                auto check = Wnd(GetDlgItem(this->status, IDC_STATUS_CHECK));
                check.MoveTo(button.X() + button.Width() + 5, 2);
                check.Resize(100, this->status.Height() - check.Y());

                auto combo = Wnd(GetDlgItem(this->status, IDC_STATUS_COMBO));
                combo.MoveTo(check.X() + check.Width(), 2);
                combo.Resize(50, 120);

                auto progress = Wnd(GetDlgItem(this->status, IDC_STATUS_PROGRESS));
                progress.MoveTo(combo.X() + combo.Width(), 2);
                progress.Resize(this->status.Width() - combo.X() - combo.Width(), this->status.Height() - progress.Y());

                break;
            }
        }

        return Control::DefWndProc(hWnd, uMsg, wParam, lParam);
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

    CheckBox check;
    if (!check.Create(this->status, IDC_STATUS_CHECK, L"Unchecked"))
    {
        return false;
    }
    check.Show();

    this->RegisterCommand(IDC_STATUS_CHECK, [this]
    {
        auto check = CheckBox(GetDlgItem(this->status, IDC_STATUS_CHECK));
        auto text = check.IsChecked() ? L"Checked" : (check.IsUnchecked() ? L"Unchecked" : L"Indeterminate");

        check.Text(text);
        this->Item(IDC_ECHO).Text(text);
        this->status.Text(text);
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

    this->RegisterCommand(IDC_STATUS_COMBO, [this]
    {
        auto combo = ComboBox(GetDlgItem(this->status, IDC_STATUS_COMBO));
        this->Position((int)combo.Data());
        return true;
    });

    ProgressBar progress;
    if (!progress.Create(this->status, IDC_STATUS_PROGRESS))
    {
        return false;
    }
    progress.Position(50);
    progress.Show();

    int positions[] = { 150, -1 };
    this->status.SetParts(2, positions);

    this->status.ClipChildren();
    this->status.Adjust();
    this->status.Show();

    return true;
}

void AppDialog::Position(int pos)
{
    auto text = to_wstring(pos);
    this->Item(IDC_ECHO).Text(text);
    this->status.Text(text);

    ((Spinner&)this->Item(IDC_SPIN)).SetPos(pos);
    ((Slider&)this->Item(IDC_SLIDER)).Position(pos);
    ((ProgressBar&)this->Item(IDC_PROGRESS)).Position(pos);
    ((ProgressBar&)Control(GetDlgItem(this->status, IDC_STATUS_PROGRESS))).Position(pos);
}