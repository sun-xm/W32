#include "D2DRenderer.h"
#include "Cleanup.h"
#include <algorithm>
#include <vector>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

using namespace std;

std::mutex D2DRenderer::mutex;
int D2DRenderer::factoryRef = 0;
ID2D1Factory*   D2DRenderer::d2d1Factory   = nullptr;
IDWriteFactory* D2DRenderer::dwriteFactory = nullptr;

D2DRenderer D2DRenderer::Create(HWND hWnd)
{
    if (!hWnd)
    {
        return D2DRenderer();
    }

    if (!AddRefFactories())
    {
        return D2DRenderer();
    }
    ONCLEANUP(factories, []{ ReleaseFactories(); });

    RECT client;
    if (!GetClientRect(hWnd, &client))
    {
        return D2DRenderer();
    }

    auto propH = D2D1::RenderTargetProperties();
    propH.type = D2D1_RENDER_TARGET_TYPE_HARDWARE;
    propH.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE;
    propH.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;
    propH.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);

    auto propD = propH;
    propD.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;

    ID2D1HwndRenderTarget* target;
    if (FAILED(D2DRenderer::d2d1Factory->CreateHwndRenderTarget(propH, D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(client.right - client.left, client.bottom - client.top)), &target)) &&
        FAILED(D2DRenderer::d2d1Factory->CreateHwndRenderTarget(propD, D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(client.right - client.left, client.bottom - client.top)), &target)))
    {
        return D2DRenderer();
    }
    ONCLEANUP(target, [target]{ target->Release(); });

    if (FAILED(target->Resize(D2D1::SizeU(client.right - client.left, client.bottom - client.top))))
    {
        return D2DRenderer();
    }

    return D2DRenderer(target);
}

D2DRenderer::D2DRenderer() : transform(D2D1::Matrix3x2F::Identity())
{
    AddRefFactories();
}

D2DRenderer::D2DRenderer(const D2DRenderer& other)
{
    *this = other;
}

D2DRenderer::D2DRenderer(ID2D1RenderTarget* target) : D2DInterface(target), transform(D2D1::Matrix3x2F::Identity())
{
    AddRefFactories();
}

D2DRenderer::~D2DRenderer()
{
    ReleaseFactories();
}

D2DRenderer& D2DRenderer::operator=(const D2DRenderer& other)
{
    *(D2DInterface*)this = other;

    this->x      = other.x;
    this->y      = other.y;
    this->brush  = other.brush;
    this->stroke = other.stroke;
    this->format = other.format;
    this->transform = other.transform;

    return *this;
}

bool D2DRenderer::ResizeTarget(int width, int height)
{
    if (!this->itf)
    {
        return false;
    }

    ID2D1HwndRenderTarget* target;
    if (FAILED(this->itf->QueryInterface(IID_PPV_ARGS(&target))))
    {
        return false;
    }
    ONCLEANUP(target, [target]{ target->Release(); });

    width  = max(0, width);
    height = max(0, height);

    return SUCCEEDED(target->Resize(D2D1::SizeU(width, height))) ? true : false;
}

bool D2DRenderer::BeginPaint()
{
    if (!this->itf)
    {
        return false;
    }

    this->itf->BeginDraw();
    return true;
}

void D2DRenderer::EndPaint()
{
    if (this->itf)
    {
        this->itf->EndDraw();
    }
}

bool D2DRenderer::Skew(float angleX, float angleY, float centerX, float centerY)
{
    if (!this->itf)
    {
        return false;
    }

    this->transform = D2D1::Matrix3x2F::Skew(angleX, angleY, D2D1::Point2F(centerX, centerY)) * this->transform;
    this->itf->SetTransform(this->transform);

    return true;
}

bool D2DRenderer::Scale(float scaleX, float scaleY, float centerX, float centerY)
{
    if (!this->itf)
    {
        return false;
    }

    this->transform = D2D1::Matrix3x2F::Scale(D2D1::SizeF(scaleX, scaleY), D2D1::Point2F(centerX, centerY)) * this->transform;
    this->itf->SetTransform(this->transform);

    return true;
}

bool D2DRenderer::Rotate(float angle, float centerX, float centerY)
{
    if (!this->itf)
    {
        return false;
    }

    this->transform = D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(centerX, centerY)) * this->transform;
    this->itf->SetTransform(this->transform);

    return true;
}

bool D2DRenderer::Translate(float translateX, float translateY)
{
    if (!this->itf)
    {
        return false;
    }

    this->transform = D2D1::Matrix3x2F::Translation(D2D1::SizeF(translateX, translateY)) * this->transform;
    this->itf->SetTransform(this->transform);

    return true;
}

bool D2DRenderer::Transform(const D2D1::Matrix3x2F& transform)
{
    if (!this->itf)
    {
        return false;
    }

    this->transform = transform * this->transform;
    this->itf->SetTransform(this->transform);

    return true;
}

bool D2DRenderer::Identity()
{
    if (!this->itf)
    {
        return false;
    }

    this->transform = D2D1::Matrix3x2F::Identity();
    this->itf->SetTransform(this->transform);

    return true;
}

void D2DRenderer::Brush(const D2DBrush& brush)
{
    this->brush = brush;
}

void D2DRenderer::From(float x, float y)
{
    this->x = x;
    this->y = y;
}

void D2DRenderer::From(const D2DPoint& point)
{
    this->x = point.X;
    this->y = point.Y;
}

bool D2DRenderer::LineTo(float x, float y)
{
    if (!this->itf || !this->brush)
    {
        return false;
    }

    this->itf->DrawLine(D2D1::Point2F(this->x, this->y), D2D1::Point2F(x, y), this->brush, this->stroke.Width(), this->stroke);

    this->x = x;
    this->y = y;

    return true;
}

bool D2DRenderer::LineTo(const D2DPoint& point)
{
    return this->LineTo(point.X, point.Y);
}

void D2DRenderer::Stroke(const D2DStroke& stroke)
{
    this->stroke = stroke;
}

bool D2DRenderer::Draw(const D2DRectangle& rectangle) const
{
    if (!this->itf || !this->brush)
    {
        return false;
    }

    if (((D2D1_ROUNDED_RECT&)rectangle).radiusX || ((D2D1_ROUNDED_RECT&)rectangle).radiusY)
    {
        this->itf->DrawRoundedRectangle(rectangle, this->brush, this->stroke.Width(), this->stroke);
    }
    else
    {
        this->itf->DrawRectangle(rectangle, this->brush, this->stroke.Width(), this->stroke);
    }

    return true;
}

bool D2DRenderer::Draw(const D2D1_ELLIPSE& ellipse) const
{
    if (!this->itf || !this->brush)
    {
        return false;
    }

    this->itf->DrawEllipse(ellipse, this->brush, this->stroke.Width(), this->stroke);

    return true;
}

bool D2DRenderer::Draw(const D2DBitmap& bitmap, const D2D1_RECT_F& rectangle, float opacity) const
{
    if (!this->itf || !bitmap)
    {
        return false;
    }

    this->itf->DrawBitmap(bitmap, rectangle, opacity);

    return true;
}

bool D2DRenderer::Draw(const D2DBitmap& bitmap, float x, float y, float cx, float cy, float opacity) const
{
    return this->Draw(bitmap, D2D1::RectF(x, y, x + cx, y + cy), opacity);
}

bool D2DRenderer::Fill(const D2DRectangle& rectangle) const
{
    if (!this->itf || !this->brush)
    {
        return false;
    }

    if (((D2D1_ROUNDED_RECT&)rectangle).radiusX || ((D2D1_ROUNDED_RECT&)rectangle).radiusY)
    {
        this->itf->FillRoundedRectangle(rectangle, this->brush);
    }
    else
    {
        this->itf->FillRectangle(rectangle, this->brush);
    }

    return true;
}

bool D2DRenderer::Fill(const D2D1_ELLIPSE& ellipse) const
{
    if (!this->itf || !this->brush)
    {
        return false;
    }

    this->itf->FillEllipse(ellipse, this->brush);

    return true;
}

void D2DRenderer::Format(const D2DFormat& format)
{
    this->format = format;
}

bool D2DRenderer::Text(const wstring& text, const D2D1_RECT_F& rectangle) const
{
    return this->Text(text.c_str(), rectangle);
}

bool D2DRenderer::Text(const wstring& text, float x, float y, float w, float h) const
{
    return this->Text(text.c_str(), x, y, w, h);
}

bool D2DRenderer::Text(const wchar_t* text, const D2D1_RECT_F& rectangle) const
{
    if (!this->itf || !this->brush || !this->format)
    {
        return false;
    }

    this->itf->DrawText(text, (UINT32)wcslen(text), this->format, rectangle, this->brush);
    return true;
}

bool D2DRenderer::Text(const wchar_t* text, float x, float y, float w, float h) const
{
    return this->Text(text, D2D1::RectF(x, y, x + w, y + h));
}

bool D2DRenderer::Clear(COLORREF rgb, float opacity)
{
    return this->Clear(GetRValue(rgb), GetGValue(rgb), GetBValue(rgb), opacity);
}

bool D2DRenderer::Clear(UCHAR r, UCHAR g, UCHAR b, float opacity)
{
    if (!this->itf)
    {
        return false;
    }

    this->itf->Clear(D2D1::ColorF(RGB(b, g, r), opacity));

    return true;
}

D2DBitmap D2DRenderer::CreateBitmap(int width, int height)
{
    if (!this->itf)
    {
        return D2DBitmap();
    }

    width  = max(0, width);
    height = max(0, height);

    ID2D1Bitmap* bitmap;
    if (FAILED(this->itf->CreateBitmap(D2D1::SizeU(width, height), D2D1::BitmapProperties(this->itf->GetPixelFormat()), &bitmap)))
    {
        return D2DBitmap();
    }
    ONCLEANUP(bitmap, [bitmap]{ bitmap->Release(); });

    return D2DBitmap(bitmap);
}

D2DBrush D2DRenderer::CreateSolidBrush(COLORREF rgb, float opacity)
{
    return this->CreateSolidBrush(GetRValue(rgb), GetGValue(rgb), GetBValue(rgb), opacity);
}

D2DBrush D2DRenderer::CreateSolidBrush(UCHAR r, UCHAR g, UCHAR b, float opacity)
{
    if (!this->itf)
    {
        return D2DBrush();
    }

    ID2D1SolidColorBrush* brush;
    if (FAILED(this->itf->CreateSolidColorBrush(D2D1::ColorF(RGB(b, g, r), opacity), &brush)))
    {
        return D2DBrush();
    }
    ONCLEANUP(brush, [brush]{ brush->Release(); });

    return D2DBrush(brush);
}

D2DBrush D2DRenderer::CreateBitmapBrush(const D2DBitmap& bitmap, float opacity, D2D1_BITMAP_INTERPOLATION_MODE interpolation, D2D1_EXTEND_MODE xMode, D2D1_EXTEND_MODE yMode)
{
    if (!this->itf)
    {
        return D2DBrush();
    }

    D2D1_BITMAP_BRUSH_PROPERTIES bitmapProperties;
    bitmapProperties.extendModeX = xMode;
    bitmapProperties.extendModeY = yMode;
    bitmapProperties.interpolationMode = interpolation;

    D2D1_BRUSH_PROPERTIES brushProperties;
    brushProperties.opacity = opacity;
    brushProperties.transform = D2D1::Matrix3x2F::Identity();

    ID2D1BitmapBrush* brush;
    if (FAILED(this->itf->CreateBitmapBrush(bitmap, &bitmapProperties, &brushProperties, &brush)))
    {
        return D2DBrush();
    }
    ONCLEANUP(brush, [brush]{ brush->Release(); });

    return D2DBrush(brush);
}

bool D2DRenderer::AddRefFactories()
{
    lock_guard<std::mutex> lock(D2DRenderer::mutex);

    if (D2DRenderer::d2d1Factory)
    {
        D2DRenderer::d2d1Factory->AddRef();
        D2DRenderer::dwriteFactory->AddRef();
    }
    else
    {
        if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &D2DRenderer::d2d1Factory)))
        {
            return false;
        }
        if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&dwriteFactory))))
        {
            D2DRenderer::d2d1Factory->Release();
            D2DRenderer::d2d1Factory = nullptr;
            return false;
        }
    }

    D2DRenderer::factoryRef++;

    return true;
}

void D2DRenderer::ReleaseFactories()
{
    lock_guard<std::mutex> lock(D2DRenderer::mutex);

    if (D2DRenderer::d2d1Factory)
    {
        D2DRenderer::d2d1Factory->Release();
        D2DRenderer::dwriteFactory->Release();

        if (!(--factoryRef))
        {
            D2DRenderer::d2d1Factory   = nullptr;
            D2DRenderer::dwriteFactory = nullptr;
        }
    }
}

D2DBitmap::D2DBitmap(ID2D1Bitmap* bitmap) : D2DInterface(bitmap)
{
}

D2DBitmap::D2DBitmap(const D2DBitmap& other) : D2DInterface(other)
{
}

int D2DBitmap::Width() const
{
    if (this->itf)
    {
        return (int)this->itf->GetPixelSize().width;
    }

    return 0;
}

int D2DBitmap::Height() const
{
    if (this->itf)
    {
        return (int)this->itf->GetPixelSize().height;
    }

    return 0;
}

bool D2DBitmap::Pixels(const uint32_t* pixels, bool premultiply)
{
    if (!pixels || !this->itf || !this->Width() || !this->Height())
    {
        return false;
    }

    if (premultiply)
    {
        vector<int> pix(this->Width() * this->Height());

        for (size_t i = 0; i < pix.size(); i++)
        {
            auto bgra = (UCHAR*)&pixels[i];
            if (bgra[3] < 255)
            {
                auto a =  bgra[3] + 1;
                auto b = (bgra[0] * a) >> 8; // approximate to (color = color * alpha / 255) but probably faster
                auto g = (bgra[1] * a) >> 8 << 8;
                auto r = (bgra[2] * a) >> 8 << 16;

                pix[i] = bgra[3] << 24 | b | g | r;
            }
            else
            {
                pix[i] = pixels[i];
            }
        }

        if (FAILED(this->itf->CopyFromMemory(nullptr, pix.data(), this->Width() * sizeof(pix[0]))))
        {
            return false;
        }
    }
    else if (FAILED(this->itf->CopyFromMemory(nullptr, pixels, this->Width() * sizeof(pixels[0]))))
    {
        return false;
    }

    return true;
}

D2DBrush::D2DBrush(ID2D1Brush* brush) : D2DInterface(brush)
{
}

D2DBrush::D2DBrush(const D2DBrush& other) : D2DInterface(other)
{
}

void D2DBrush::Opacity(float opacity)
{
    if (this->itf)
    {
        this->itf->SetOpacity(opacity);
    }
}

void D2DBrush::Transform(const D2D1_MATRIX_3X2_F& transform)
{
    if (this->itf)
    {
        this->itf->SetTransform(transform);
    }
}

D2DStroke::D2DStroke(const D2DStroke& other)
{
    *this = other;
}

D2DStroke::D2DStroke(float width, ID2D1StrokeStyle* style) : D2DInterface(style), width(width)
{
}

D2DStroke& D2DStroke::operator=(const D2DStroke& other)
{
    *(D2DInterface*)this = other;
    this->width = other.width;
    return *this;
}

float D2DStroke::Width() const
{
    return this->width;
}

void D2DStroke::Width(float width)
{
    this->width = width;
}

D2DFormat D2DFormat::Create(const wchar_t* family, float size, DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch)
{
    if (!D2DRenderer::AddRefFactories())
    {
        return D2DFormat();
    }
    ONCLEANUP(factories, []{ D2DRenderer::ReleaseFactories(); });

    IDWriteTextFormat* format;
    if (FAILED(D2DRenderer::dwriteFactory->CreateTextFormat(family, nullptr, weight, style, stretch, size, L"", &format)))
    {
        return D2DFormat();
    }
    ONCLEANUP(format, [format]{ format->Release(); });

    return D2DFormat(format);
}

D2DFormat::D2DFormat(IDWriteTextFormat* format) : D2DInterface(format)
{
}

D2DFormat::D2DFormat(const D2DFormat& other) : D2DInterface(other)
{
}

void D2DFormat::TextAlign(DWRITE_TEXT_ALIGNMENT alignment)
{
    if (this->itf)
    {
        this->itf->SetTextAlignment(alignment);
    }
}

void D2DFormat::ParaAlign(DWRITE_PARAGRAPH_ALIGNMENT alignment)
{
    if (this->itf)
    {
        this->itf->SetParagraphAlignment(alignment);
    }
}