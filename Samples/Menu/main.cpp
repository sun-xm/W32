#include <Application.h>
#include "MainWindow.h"

int APIENTRY wWinMain(HINSTANCE instance, HINSTANCE, LPWSTR, int nCmdShow)
{
    return Application(instance).Run(MainWindow(), nCmdShow);
}