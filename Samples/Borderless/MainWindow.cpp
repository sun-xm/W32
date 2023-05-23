#include "MainWindow.h"

bool MainWindow::OnCreated()
{
    if (!Window::OnCreated())
    {
        return false;
    }

    this->RegisterMessage(WM_MOUSEMOVE, [this]
    {
        POINT pos;
        GetCursorPos(&pos);

        if (MK_LBUTTON & this->wparam)
        {
            auto x = pos.x - this->pos.x;
            auto y = pos.y - this->pos.y;
            this->MoveTo(this->X() + x, this->Y() + y);
        }

        this->pos = pos;

        return 0;
    });

    this->RegisterMessage(WM_KEYUP, [this]
    {
        if (VK_ESCAPE == this->wparam)
        {
            this->Close();
            return (LRESULT)0;
        }

        return DefWindowProcW(*this, WM_KEYUP, this->wparam, this->lparam);
    });

    this->renderer = D2DRenderer::Create(*this);
    if (!this->renderer)
    {
        return false;
    }

    auto format = D2DFormat::Create(L"Segoe UI", 50.f);
    format.TextAlign(DWRITE_TEXT_ALIGNMENT_CENTER);
    format.ParaAlign(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

    this->renderer.Format(format);
    this->renderer.Brush(this->renderer.CreateSolidBrush(RGB(0, 0, 0)));

    this->Style(WS_OVERLAPPED | WS_POPUP);
    this->Resize(400, 400);

    return true;
}

void MainWindow::OnPaint()
{
    if (this->renderer.BeginPaint())
    {
        this->renderer.Clear(RGB(0x87, 0xCE, 0xFA));
        this->renderer.Text(L"A borderless window", 0, 0, (float)this->ClientW(), (float)this->ClientH());
        this->renderer.EndPaint();
    }

    Window::OnPaint();
}

void MainWindow::OnSize()
{
    this->renderer.ResizeTarget(this->ClientW(), this->ClientH());
    Window::OnSize();
}