#include "DDCRenderer.h"
#include "Cleanup.h"

DDCRenderer DDCRenderer::Create()
{
    if (!AddRefFactories())
    {
        return DDCRenderer();
    }
    ONCLEANUP(factories, []{ ReleaseFactories(); });

    auto prop = D2D1::RenderTargetProperties();
    prop.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
    prop.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE;
    prop.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;
    prop.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);

    ID2D1DCRenderTarget* target;
    if (FAILED(DDCRenderer::d2d1Factory->CreateDCRenderTarget(&prop, &target)))
    {
        return DDCRenderer();
    }
    ONCLEANUP(target, [target]{ target->Release(); });

    return DDCRenderer(target);
}

DDCRenderer::DDCRenderer(const DDCRenderer& other) : D2DRenderer(other)
{
}

DDCRenderer::DDCRenderer(ID2D1DCRenderTarget* target) : D2DRenderer(target)
{
}

bool DDCRenderer::ResizeTarget(int width, int height)
{
    if (!this->itf)
    {
        return false;
    }

    if (this->dc.Width() != width || this->dc.Height() != height)
    {
        ID2D1DCRenderTarget* target;
        if (FAILED(this->itf->QueryInterface(IID_PPV_ARGS(&target))))
        {
            return false;
        }
        ONCLEANUP(target, [target]{ target->Release(); });

        this->dc = D2DDc(width, height);
        RECT r = { 0, 0, width, height };

        if (FAILED(target->BindDC(this->dc, &r)))
        {
            this->dc = D2DDc();
            return false;
        }
    }

    return true;
}

HDC DDCRenderer::GetDC() const
{
    return (::HDC)this->dc;
}

DDCRenderer& DDCRenderer::operator=(const DDCRenderer& other)
{
    *(D2DRenderer*)this = other;
    return *this;
}

D2DDc::D2DDc() : hdc(nullptr), bmp(nullptr), old(nullptr), w(0), h(0)
{
}

D2DDc::D2DDc(int width, int height)
{
    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = width * height * 4;
    bmi.bmiHeader.biXPelsPerMeter = 3780;
    bmi.bmiHeader.biYPelsPerMeter = 3780;
    bmi.bmiHeader.biClrUsed = 0;
    bmi.bmiHeader.biClrImportant = 0;

    void* bits;
    this->bmp = CreateDIBSection(nullptr, &bmi, DIB_RGB_COLORS, &bits, 0, 0);
    this->hdc = this->bmp ? CreateCompatibleDC(nullptr) : nullptr;
    this->old = this->hdc ? SelectObject(this->hdc, this->bmp) : nullptr;
    this->w   = this->hdc ? width  : 0;
    this->h   = this->hdc ? height : 0;
}

D2DDc::D2DDc(D2DDc&& other)
{
    *this = std::move(other);
}

D2DDc::~D2DDc()
{
    if (this->hdc)
    {
        SelectObject(this->hdc, this->old);
        DeleteDC(this->hdc);
    }

    if (this->bmp)
    {
        DeleteObject(this->bmp);
    }
}

D2DDc::operator bool() const
{
    return this->hdc ? true : false;
}

D2DDc::operator HDC() const
{
    return this->hdc;
}

D2DDc& D2DDc::operator=(D2DDc&& other)
{
    if (this->hdc)
    {
        SelectObject(this->hdc, this->old);
        DeleteDC(this->hdc);
    }

    if (this->bmp)
    {
        DeleteObject(this->bmp);
    }

    this->hdc = other.hdc;
    this->bmp = other.bmp;
    this->old = other.old;
    this->w   = other.w;
    this->h   = other.h;

    other.hdc = nullptr;
    other.bmp = nullptr;
    other.old = nullptr;
    other.w   = 0;
    other.h   = 0;

    return *this;
}

int D2DDc::Width() const
{
    return this->w;
}

int D2DDc::Height() const
{
    return this->h;
}