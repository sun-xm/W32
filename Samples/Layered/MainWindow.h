#pragma once

#include <Window.h>
#include <DDCRenderer.h>

class MainWindow : public Window
{
protected:
    bool OnCreated() override;
    void OnSize() override;

private:
    void Update();

private:
    POINT pos;
    DDCRenderer renderer;
};