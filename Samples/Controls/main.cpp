#include <App.h>
#include "AppDialog.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
    return App(hInstance).Run(AppDialog(), nCmdShow);
}