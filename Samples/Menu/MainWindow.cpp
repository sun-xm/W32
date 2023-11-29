#include "MainWindow.h"
#include "resource.h"

#define REGISTER(id)    RegisterCommand(id, [this]{ this->OnMenu(id); return true; })

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
    REGISTER(ID_MENU_ITEM2);
    REGISTER(ID_MENU_ITEM3);

    return true;
}

void MainWindow::OnSize()
{
    this->bar.Adjust();

    Window::OnSize();
}

void MainWindow::OnMenu(int id)
{
    this->bar.Text(L"Item" + std::to_wstring(id - ID_MENU_ITEM0) + L" clicked");

    auto item = this->menu.Item(id);
    item.Check(!item.IsChecked());
}