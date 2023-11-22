#include "MainWindow.h"
#include "resource.h"

bool MainWindow::OnCreated()
{
    if (!Window::OnCreated())
    {
        return false;
    }

    if (!this->menu.Create(IDR_MENU))
    {
        return false;
    }

    this->SetMenu(this->menu);

    this->RegisterMessage(WM_RBUTTONDOWN, [this]
    {
        POINT pt;
        GetCursorPos(&pt);

        auto item = this->menu.Item(0);
        item.EnableItem(ID_MENU_ITEM0, false);
        item.CheckItem(ID_MENU_ITEM1, true);
        item.Popup(*this, pt.x, pt.y);

        return 0;
    });

    return true;
}