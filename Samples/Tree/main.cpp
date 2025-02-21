#include <Application.h>
#include "MainDialog.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
    return Application(hInstance).Run(MainDialog(), nCmdShow);
}