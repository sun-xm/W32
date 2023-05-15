#include <App.h>
#include <MainWindow.h>

int APIENTRY wWinMain(HINSTANCE instance, HINSTANCE, LPWSTR, int nCmdShow)
{
    return App(instance).Run(MainWindow(), nCmdShow);
}