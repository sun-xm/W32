#pragma once

#include <Window.h>
#include <Menu.h>

class MainWindow : public Window
{
protected:
    bool OnCreated() override;

private:
    Menu menu;
};