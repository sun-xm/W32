#pragma once

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#include <algorithm>
#include <Cleanup.h>
#include <d2d1.h>
#include <dwrite.h>
#include <mutex>
#include <Windows.h>

class D2DFactory
{
public:
    static ID2D1Factory* Get()
    {
        static D2DFactory instance;
        return instance.factory;
    }

private:
    D2DFactory()
    {
        D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &this->factory);
    }
   ~D2DFactory()
    {
        if (this->factory)
        {
            this->factory->Release();
        }
    }

private:
    ID2D1Factory* factory;
};

class DWriteFactory
{
public:
    static IDWriteFactory* Get()
    {
        static DWriteFactory instance;
        return instance.factory;
    }

private:
    DWriteFactory()
    {
        DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&this->factory));
    }
   ~DWriteFactory()
    {
        if (this->factory)
        {
            this->factory->Release();
        }
    }

    IDWriteFactory* factory;
};

class D2DPoint
{
public:
    D2DPoint()
    {
        this->X = 0;
        this->Y = 0;
    }

    D2DPoint(float x, float y)
    {
        this->X = x;
        this->Y = y;
    }

    float X;
    float Y;
};

class D2DRectangle
{
public:
    D2DRectangle()
    {
        this->rect.rect    = D2D1::RectF();
        this->rect.radiusX = 0;
        this->rect.radiusY = 0;
    }

    D2DRectangle(float left, float top, float right, float bottom, float radius = 0.f)
    {
        this->rect.rect    = D2D1::RectF(left, top, right, bottom);
        this->rect.radiusX = radius;
        this->rect.radiusY = radius;
    }

    D2DRectangle(float left, float top, float right, float bottom, float radiusX, float radiusY)
    {
        this->rect.rect    = D2D1::RectF(left, top, right, bottom);
        this->rect.radiusX = radiusX;
        this->rect.radiusY = radiusY;
    }

    D2DRectangle(float width, float height, const D2DPoint& center = D2DPoint(0.f, 0.f), float radius = 0.f)
    {
        this->rect.rect    = D2D1::RectF(center.X - width  * .5f, center.Y - height * .5f, center.X + width  * .5f, center.Y + height * .5f);
        this->rect.radiusX = radius;
        this->rect.radiusY = radius;
    }

    D2DRectangle(float width, float height, const D2DPoint& center, float radiusX, float radiusY)
    {
        this->rect.rect    = D2D1::RectF(center.X - width  * .5f, center.Y - height * .5f, center.X + width  * .5f, center.Y + height * .5f);
        this->rect.radiusX = radiusX;
        this->rect.radiusY = radiusY;
    }

    operator const D2D1_RECT_F&() const
    {
        return this->rect.rect;
    }

    operator const D2D1_ROUNDED_RECT&() const
    {
        return this->rect;
    }

    float L() const
    {
        return this->rect.rect.left;
    }

    float R() const
    {
        return this->rect.rect.right;
    }

    float T() const
    {
        return this->rect.rect.top;
    }

    float B() const
    {
        return this->rect.rect.bottom;
    }

    float W() const
    {
        return this->rect.rect.right - this->rect.rect.left;
    }

    float H() const
    {
        return this->rect.rect.bottom - this->rect.rect.top;
    }

    float Rx() const
    {
        return this->rect.radiusX;
    }

    float Ry() const
    {
        return this->rect.radiusY;
    }

private:
    D2D1_ROUNDED_RECT rect;
};

class D2DEllipse
{
public:
    D2DEllipse(float left, float top, float right, float bottom)
    {
        auto center = D2D1::Point2F((left + right) * .5f, (top + bottom) * .5f);
        auto radius = D2D1::Point2F((right - left) * .5f, (bottom - top) * .5f);
        this->Value = D2D1::Ellipse(center, radius.x, radius.y);
    }

    D2DEllipse(float radiusX, float radiusY, const D2DPoint& center = D2DPoint(0.f, 0.f))
    {
        this->Value = D2D1::Ellipse(D2D1::Point2F(center.X, center.Y), radiusX, radiusY);
    }

    operator D2D1_ELLIPSE() const
    {
        return this->Value;
    }

    D2D1_ELLIPSE Value;
};

template<typename T>
class D2DInterface
{
public:
    D2DInterface() : itf(nullptr)
    {
    }
    D2DInterface(const D2DInterface& other) : D2DInterface()
    {
        *this = other;
    }
    D2DInterface(T* itf) : itf(itf)
    {
        if (itf)
        {
            itf->AddRef();
        }
    }
   ~D2DInterface()
    {
        if (this->itf)
        {
            this->itf->Release();
        }
    }

    operator bool() const
    {
        return this->itf ? true : false;
    }
    operator T*() const
    {
        return this->itf;
    }

    D2DInterface& operator=(const D2DInterface& other)
    {
        if (this->itf)
        {
            this->itf->Release();
        }

        this->itf = other.itf;

        if (this->itf)
        {
            this->itf->AddRef();
        }

        return *this;
    }

protected:
    T* itf;
};

class D2DBitmap : public D2DInterface<ID2D1Bitmap>
{
public:
    D2DBitmap() = default;
    D2DBitmap(ID2D1Bitmap* bitmap) : D2DInterface(bitmap) {}
    D2DBitmap(const D2DBitmap& other) : D2DInterface(other) {}

    int Width() const
    {
        if (this->itf)
        {
            return (int)this->itf->GetPixelSize().width;
        }

        return 0;
    }
    int Height() const
    {
        if (this->itf)
        {
            return (int)this->itf->GetPixelSize().height;
        }

        return 0;
    }
    bool Pixels(const uint32_t* pixels, bool premultiply = false)
    {
        if (!pixels || !this->itf || !this->Width() || !this->Height())
        {
            return false;
        }

        if (premultiply)
        {
            std::vector<int> pix(this->Width() * this->Height());

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
};

class D2DBrush : public D2DInterface<ID2D1Brush>
{
public:
    D2DBrush() = default;
    D2DBrush(ID2D1Brush* brush) : D2DInterface(brush) {}
    D2DBrush(const D2DBrush& other) : D2DInterface(other) {}

    void Opacity(float opacity)
    {
        if (this->itf)
        {
            this->itf->SetOpacity(opacity);
        }
    }
    void Transform(const D2D1_MATRIX_3X2_F& transform)
    {
        if (this->itf)
        {
            this->itf->SetTransform(transform);
        }
    }
};

class D2DStroke : public D2DInterface<ID2D1StrokeStyle>
{
public:
    D2DStroke(const D2DStroke& other) { *this = other; }
    D2DStroke(float width = 1.f, ID2D1StrokeStyle* style = nullptr) : width(width), D2DInterface(style) {}

    float Width() const { return this->width; }
    void Width(float) { this->width = width; }

    D2DStroke& operator=(const D2DStroke& other)
    {
        *(D2DInterface*)this = other;
        this->width = other.width;
        return *this;
    }

private:
    float width;
};

class D2DFormat : public D2DInterface<IDWriteTextFormat>
{
public:
    static D2DFormat Create(const wchar_t* family, float size, DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH stretch = DWRITE_FONT_STRETCH_NORMAL)
    {
        IDWriteTextFormat* format;
        if (FAILED(DWriteFactory::Get()->CreateTextFormat(family, nullptr, weight, style, stretch, size, L"", &format)))
        {
            return D2DFormat();
        }
        ONCLEANUP(format, [format]{ format->Release(); });

        return D2DFormat(format);
    }

    D2DFormat() = default;
    D2DFormat(IDWriteTextFormat* format) : D2DInterface(format) {}
    D2DFormat(const D2DFormat& other) : D2DInterface(other) {}

    void TextAlign(DWRITE_TEXT_ALIGNMENT alignment)
    {
        if (this->itf)
        {
            this->itf->SetTextAlignment(alignment);
        }
    }
    void ParaAlign(DWRITE_PARAGRAPH_ALIGNMENT alignment)
    {
        if (this->itf)
        {
            this->itf->SetParagraphAlignment(alignment);
        }
    }
};

class D2DRenderer : public D2DInterface<ID2D1RenderTarget>
{
    friend class D2DFormat;

public:
    static D2DRenderer Create(HWND hWnd)
    {
        if (!hWnd)
        {
            return D2DRenderer();
        }

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
        if (FAILED(D2DFactory::Get()->CreateHwndRenderTarget(propH, D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(client.right - client.left, client.bottom - client.top)), &target)) &&
            FAILED(D2DFactory::Get()->CreateHwndRenderTarget(propD, D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(client.right - client.left, client.bottom - client.top)), &target)))
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

    D2DRenderer() : transform(D2D1::Matrix3x2F::Identity()) {}
    D2DRenderer(const D2DRenderer& other) { *this = other; }
    D2DRenderer(ID2D1RenderTarget* target) : D2DInterface(target), transform(D2D1::Matrix3x2F::Identity()) {}
    virtual ~D2DRenderer() {}

    D2DRenderer& operator=(const D2DRenderer& other)
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

    bool ResizeTarget(int width, int height)
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

        width  = (std::max)(0, width);
        height = (std::max)(0, height);

        return SUCCEEDED(target->Resize(D2D1::SizeU(width, height))) ? true : false;
    }
    bool BeginPaint()
    {
        if (!this->itf)
        {
            return false;
        }

        this->itf->BeginDraw();
        return true;
    }
    void EndPaint()
    {
        if (this->itf)
        {
            this->itf->EndDraw();
        }
    }

    bool Skew(float angleX, float angleY, float centerX, float centerY)
    {
        if (!this->itf)
        {
            return false;
        }

        this->transform = this->transform * D2D1::Matrix3x2F::Skew(angleX, angleY, D2D1::Point2F(centerX, centerY));
        this->itf->SetTransform(this->transform);

        return true;
    }
    bool Scale(float scaleX, float scaleY, float centerX, float centerY)
    {
        if (!this->itf)
        {
            return false;
        }

        this->transform = this->transform * D2D1::Matrix3x2F::Scale(D2D1::SizeF(scaleX, scaleY), D2D1::Point2F(centerX, centerY));
        this->itf->SetTransform(this->transform);

        return true;
    }
    bool Rotate(float angle, float centerX = 0.f, float centerY = 0.f)
    {
        if (!this->itf)
        {
            return false;
        }

        this->transform = this->transform * D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(centerX, centerY));
        this->itf->SetTransform(this->transform);

        return true;
    }
    bool Translate(float translateX, float translateY)
    {
        if (!this->itf)
        {
            return false;
        }

        this->transform = this->transform * D2D1::Matrix3x2F::Translation(D2D1::SizeF(translateX, translateY));
        this->itf->SetTransform(this->transform);

        return true;
    }
    bool Transform(const D2D1::Matrix3x2F& transform)
    {
        if (!this->itf)
        {
            return false;
        }

        this->transform = this->transform * transform;
        this->itf->SetTransform(this->transform);

        return true;
    }
    bool Identity()
    {
        if (!this->itf)
        {
            return false;
        }

        this->transform = D2D1::Matrix3x2F::Identity();
        this->itf->SetTransform(this->transform);

        return true;
    }

    void Brush(const D2DBrush& brush) { this->brush = brush; }

    void From(float x, float y)
    {
        this->x = x;
        this->y = y;
    }
    void From(const D2DPoint& point)
    {
        this->x = point.X;
        this->y = point.Y;
    }
    bool LineTo(float x, float y)
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
    bool LineTo(const D2DPoint& point) { return this->LineTo(point.X, point.Y); }
    void Stroke(const D2DStroke& stroke) { this->stroke = stroke; }

    bool Draw(const D2DRectangle& rectangle) const
    {
        if (!this->itf || !this->brush)
        {
            return false;
        }

        if (static_cast<D2D1_ROUNDED_RECT>(rectangle).radiusX > 0 || static_cast<D2D1_ROUNDED_RECT>(rectangle).radiusY > 0)
        {
            this->itf->DrawRoundedRectangle(rectangle, this->brush, this->stroke.Width(), this->stroke);
        }
        else
        {
            this->itf->DrawRectangle(rectangle, this->brush, this->stroke.Width(), this->stroke);
        }

        return true;
    }
    bool Draw(const D2D1_ELLIPSE& ellipse) const
    {
        if (!this->itf || !this->brush)
        {
            return false;
        }

        this->itf->DrawEllipse(ellipse, this->brush, this->stroke.Width(), this->stroke);

        return true;
    }
    bool Draw(const D2DBitmap& bitmap, const D2D1_RECT_F& rectangle, float opacity = 1.f) const
    {
        if (!this->itf || !bitmap)
        {
            return false;
        }

        this->itf->DrawBitmap(bitmap, rectangle, opacity);

        return true;
    }
    bool Draw(const D2DBitmap& bitmap, float x, float y, float cx, float cy, float opacity = 1.f) const
    {
        return this->Draw(bitmap, D2D1::RectF(x, y, x + cx, y + cy), opacity);
    }
    bool Fill(const D2DRectangle& rectangle) const
    {
        if (!this->itf || !this->brush)
        {
            return false;
        }

        if (static_cast<D2D1_ROUNDED_RECT>(rectangle).radiusX || static_cast<D2D1_ROUNDED_RECT>(rectangle).radiusY)
        {
            this->itf->FillRoundedRectangle(rectangle, this->brush);
        }
        else
        {
            this->itf->FillRectangle(rectangle, this->brush);
        }

        return true;
    }
    bool Fill(const D2D1_ELLIPSE& ellipse) const
    {
        if (!this->itf || !this->brush)
        {
            return false;
        }

        this->itf->FillEllipse(ellipse, this->brush);

        return true;
    }

    void Format(const D2DFormat& format) { this->format = format; }
    bool Text(const std::wstring& text, const D2D_RECT_F& rectangle) const { return this->Text(text.c_str(), rectangle); }
    bool Text(const std::wstring& text, float x, float y, float w, float h) const { return this->Text(text.c_str(), x, y, w, h); }
    bool Text(const wchar_t* text, const D2D1_RECT_F& rectangle) const
    {
        if (!this->itf || !this->brush || !this->format)
        {
            return false;
        }

        this->itf->DrawText(text, (UINT32)wcslen(text), this->format, rectangle, this->brush);
        return true;
    }
    bool Text(const wchar_t* text, float x, float y, float w, float h) const { return this->Text(text, D2D1::RectF(x, y, x + w, y + h)); }

    bool Clear(COLORREF rgb, float opacity = 1.f) { return this->Clear(GetRValue(rgb), GetGValue(rgb), GetBValue(rgb), opacity); }
    bool Clear(UCHAR r, UCHAR g, UCHAR b, float opacity = 1.f)
    {
        if (!this->itf)
        {
            return false;
        }

        this->itf->Clear(D2D1::ColorF(RGB(b, g, r), opacity));

        return true;
    }

    D2DBitmap CreateBitmap(int width, int height)
    {
        if (!this->itf)
        {
            return D2DBitmap();
        }

        width  = (std::max)(0, width);
        height = (std::max)(0, height);

        ID2D1Bitmap* bitmap;
        if (FAILED(this->itf->CreateBitmap(D2D1::SizeU(width, height), D2D1::BitmapProperties(this->itf->GetPixelFormat()), &bitmap)))
        {
            return D2DBitmap();
        }
        ONCLEANUP(bitmap, [bitmap]{ bitmap->Release(); });

        return D2DBitmap(bitmap);
    }
    D2DBrush  CreateSolidBrush(COLORREF rgb, float opacity = 1.f) { return this->CreateSolidBrush(GetRValue(rgb), GetGValue(rgb), GetBValue(rgb), opacity);}
    D2DBrush  CreateSolidBrush(UCHAR r, UCHAR g, UCHAR b, float opacity = 1.f)
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
    D2DBrush  CreateBitmapBrush(const D2DBitmap& bitmap, float opacity = 1.f, D2D1_BITMAP_INTERPOLATION_MODE interpolation = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, D2D1_EXTEND_MODE xMode = D2D1_EXTEND_MODE_WRAP, D2D1_EXTEND_MODE yMode = D2D1_EXTEND_MODE_WRAP)
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

    static std::mutex& Mutex()
    {
        static std::mutex mutex;
        return mutex;
    }

protected:
    float x, y;

    D2DFormat format;
    D2DStroke stroke;
    D2DBrush  brush;

    D2D1_MATRIX_3X2_F transform;
};