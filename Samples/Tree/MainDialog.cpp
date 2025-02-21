#include "MainDialog.h"
#include "resource.h"
#include <TreeView.h>

MainDialog::MainDialog() : Dialog(IDD_MAIN)
{
}

bool MainDialog::OnCreated()
{
    if (!Dialog::OnCreated())
    {
        return false;
    }

    TreeView tree(this->Item(IDC_TREE));
    auto hello = tree.Add(L"Hello");
    auto world = tree.Add(L"World", hello);
    world.Text(L"whaha");

    return true;
}