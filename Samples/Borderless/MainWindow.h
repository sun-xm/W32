#pragma once

#include <Window.h>
#include <D2DRenderer.h>

class MainWindow : public Window
{
protected:
    bool OnCreated() override;
    void OnPaint() override;
    void OnSize() override;

private:
    POINT pos;
    D2DRenderer renderer;
};