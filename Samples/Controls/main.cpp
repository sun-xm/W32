#include <Application.h>
#include "AppDialog.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
    return Application(hInstance).Run(AppDialog(), nCmdShow);
}