#include "MainWindow.h"
#include "resource.h"

#define REGISTER(id)    RegisterCommand(id, [this]{ this->OnCheck(id); return true; })
#define RADIO(first, last, id) RegisterCommand(id, [this]{ auto radio = this->menu.RadioItems(first, last); radio.Check(id); this->OnRadio(radio.GetChecked()); return true; })

bool MainWindow::OnCreated()
{
    if (!Window::OnCreated())
    {
        return false;
    }

    if (!this->bar.Create(*this, 1000))
    {
        return false;
    }
    this->bar.Show();

    if (!this->menu.Create(IDR_MENU))
    {
        return false;
    }

    this->SetMenu(this->menu);

    this->RegisterMessage(WM_RBUTTONDOWN, [this]
    {
        POINT pt;
        GetCursorPos(&pt);

        this->menu.SubMenuContains(ID_MENU_ITEM2).Popup(*this, pt.x, pt.y);

        return 0;
    });

    REGISTER(ID_MENU_ITEM0);
    REGISTER(ID_MENU_ITEM1);

    RADIO(ID_MENU_ITEM2, ID_MENU_ITEM3, ID_MENU_ITEM2);
    RADIO(ID_MENU_ITEM2, ID_MENU_ITEM3, ID_MENU_ITEM3);

    return true;
}

void MainWindow::OnSize()
{
    this->bar.Adjust();

    Window::OnSize();
}

void MainWindow::OnCheck(int id)
{
    auto item = this->menu.Item(id);
    item.Check(!item.IsChecked());
    this->bar.Text(L"Item" + std::to_wstring(id - ID_MENU_ITEM0) + (item.IsChecked() ? L" checked" : L" unchecked"));
}

void MainWindow::OnRadio(int id)
{
    this->bar.Text(L"Item" + std::to_wstring(id - ID_MENU_ITEM0) + L" is radio selected");
}