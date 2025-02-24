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
    hello.Clear();
    // tree.Root().Clear();

    return true;
}