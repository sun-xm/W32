#pragma once

#include <Window.h>
#include <Menu.h>
#include <StatusBar.h>

class MainWindow : public Window
{
protected:
    bool OnCreated() override;
    void OnSize() override;

private:
    void OnMenu(int id);

private:
    Menu menu;
    StatusBar bar;
};