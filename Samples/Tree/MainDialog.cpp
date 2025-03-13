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
    auto hello = tree.Root().Add(L"Hello");
    auto world = hello.Add(L"World");

    function<bool(TreeViewItem&)> en = [&en](TreeViewItem& item)
    {
        wcout << item.Text() << endl;
        return item.EnumerateChild(en);
    };
    tree.Root().EnumerateChild(en);

    return true;
}