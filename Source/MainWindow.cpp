#include "MainWindow.h"

void MainWindow::OnDestroy()
{
    PostQuitMessage(0);
    return Window::OnDestroy();
}