#include "MainDialog.h"
#include "resource.h"
#include <TreeView.h>
#include <iostream>

using namespace std;

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

    function<bool(TreeViewItem&)> onItem = [&](TreeViewItem& item)
    {
        wcout << item.Text() << endl;
        return item.EnumerateChild(onItem);
    };

    tree.Root().EnumerateChild(onItem);

    return true;
}