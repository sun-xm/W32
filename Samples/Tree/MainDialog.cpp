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
    auto wahah = tree.Add(L"wahah", (void*)"wahaha", hello);

    auto text = (char*)tree.Data(wahah);

    return true;
}