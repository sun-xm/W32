#include "MainWindow.h"

bool MainWindow::OnCreated()
{
    if (!Window::OnCreated())
    {
        return false;
    }

    this->Style(WS_OVERLAPPED | WS_POPUP);
    this->StyleEx(WS_EX_LAYERED);

    this->renderer = DDCRenderer::Create();
    if (!this->renderer)
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

    this->Resize(400, 400);

    return true;
}

void MainWindow::OnSize()
{
    this->Update();
}

void MainWindow::Update()
{
    auto w = this->ClientW();
    auto h = this->ClientH();
    this->renderer.ResizeTarget(w, h);

    if (this->renderer.BeginPaint())
    {
        this->renderer.Brush(this->renderer.CreateSolidBrush(RGB(0x87, 0xCE, 0xFA), .5f));
        this->renderer.Fill(D2DEllipse(0.f, 0.f, (float)w, (float)h));
        this->renderer.Brush(this->renderer.CreateSolidBrush(RGB(0, 0, 0)));

        auto format = D2DFormat::Create(L"Segoe UI", 50.f);
        format.TextAlign(DWRITE_TEXT_ALIGNMENT_CENTER);
        format.ParaAlign(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
        this->renderer.Format(format);
        this->renderer.Text(L"A layered transparent window", 0, 0, (float)w, (float)h);
        this->renderer.EndPaint();

        auto sp = POINT{ 0, 0 };
        auto dp = POINT{ this->X(), this->Y() };
        auto size = SIZE{ w, h };

        BLENDFUNCTION blend = {};
        blend.AlphaFormat = AC_SRC_ALPHA;
        blend.SourceConstantAlpha = 255;

        UPDATELAYEREDWINDOWINFO ulwi = {};
        ulwi.cbSize  = sizeof(ulwi);
        ulwi.psize   = &size;
        ulwi.hdcSrc  = this->renderer.GetDC();
        ulwi.pptSrc  = &sp;
        ulwi.pptDst  = &dp;
        ulwi.pblend  = &blend;
        ulwi.dwFlags = ULW_ALPHA;

        UpdateLayeredWindowIndirect(*this, &ulwi);
    }
}