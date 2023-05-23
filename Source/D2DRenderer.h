#pragma once

#define NOMINMAX
#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include <mutex>

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

    operator D2D1_RECT_F() const
    {
        return this->rect.rect;
    }

    operator D2D1_ROUNDED_RECT() const
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
    D2DBitmap(ID2D1Bitmap*);
    D2DBitmap(const D2DBitmap&);

    int Width() const;
    int Height() const;
    bool Pixels(const uint32_t* pixels, bool premultiply = false);
};

class D2DBrush : public D2DInterface<ID2D1Brush>
{
public:
    D2DBrush() = default;
    D2DBrush(ID2D1Brush*);
    D2DBrush(const D2DBrush&);

    void Opacity(float);
    void Transform(const D2D1_MATRIX_3X2_F&);
};

class D2DStroke : public D2DInterface<ID2D1StrokeStyle>
{
public:
    D2DStroke(const D2DStroke&);
    D2DStroke(float width = 1.f, ID2D1StrokeStyle* style = nullptr);

    float Width() const;
    void Width(float);

    D2DStroke& operator=(const D2DStroke&);

private:
    float width;
};

class D2DFormat : public D2DInterface<IDWriteTextFormat>
{
public:
    static D2DFormat Create(const wchar_t* family, float size, DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH stretch = DWRITE_FONT_STRETCH_NORMAL);

    D2DFormat() = default;
    D2DFormat(IDWriteTextFormat*);
    D2DFormat(const D2DFormat&);

    void TextAlign(DWRITE_TEXT_ALIGNMENT);
    void ParaAlign(DWRITE_PARAGRAPH_ALIGNMENT);
};

class D2DRenderer : public D2DInterface<ID2D1RenderTarget>
{
    friend class D2DFormat;

public:
    static D2DRenderer Create(HWND hWnd);

    D2DRenderer();
    D2DRenderer(const D2DRenderer&);
    D2DRenderer(ID2D1RenderTarget*);
   ~D2DRenderer();

    D2DRenderer& operator=(const D2DRenderer&);

    bool ResizeTarget(int width, int height);
    bool BeginPaint();
    void EndPaint();

    bool Skew(float angleX, float angleY, float centerX, float centerY);
    bool Scale(float scaleX, float scaleY, float centerX, float centerY);
    bool Rotate(float angle, float centerX = 0.f, float centerY = 0.f);
    bool Translate(float translateX, float translateY);
    bool Transform(const D2D1::Matrix3x2F& transform);
    bool Identity();

    void Brush(const D2DBrush& brush);

    void From(float x, float y);
    void From(const D2DPoint&);
    bool LineTo(float x, float y);
    bool LineTo(const D2DPoint&);
    void Stroke(const D2DStroke&);

    bool Draw(const D2DRectangle& rectangle) const;
    bool Draw(const D2D1_ELLIPSE& ellipse) const;
    bool Draw(const D2DBitmap& bitmap, const D2D1_RECT_F& rectangle, float opacity = 1.f) const;
    bool Draw(const D2DBitmap& bitmap, float x, float y, float cx, float cy, float opacity = 1.f) const;
    bool Fill(const D2DRectangle& rectangle) const;
    bool Fill(const D2D1_ELLIPSE& ellipse) const;

    void Format(const D2DFormat& format);
    bool Text(const std::wstring& text, const D2D_RECT_F& rectangle) const;
    bool Text(const std::wstring& text, float x, float y, float w, float h) const;
    bool Text(const wchar_t* text, const D2D1_RECT_F& rectangle) const;
    bool Text(const wchar_t* text, float x, float y, float w, float h) const;

    bool Clear(COLORREF rgb, float opacity = 1.f);
    bool Clear(UCHAR r, UCHAR g, UCHAR b, float opacity = 1.f);

    D2DBitmap CreateBitmap(int width, int height);
    D2DBrush  CreateSolidBrush(COLORREF rgb, float opacity = 1.f);
    D2DBrush  CreateSolidBrush(UCHAR r, UCHAR g, UCHAR b, float opacity = 1.f);
    D2DBrush  CreateBitmapBrush(const D2DBitmap&, float opacity = 1.f, D2D1_BITMAP_INTERPOLATION_MODE interpolation = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, D2D1_EXTEND_MODE xMode = D2D1_EXTEND_MODE_WRAP, D2D1_EXTEND_MODE yMode = D2D1_EXTEND_MODE_WRAP);

protected:
    static bool AddRefFactories();
    static void ReleaseFactories();

protected:
    float x, y;

    D2DFormat format;
    D2DStroke stroke;
    D2DBrush  brush;

    D2D1_MATRIX_3X2_F transform;

    static std::mutex mutex;

    static int             factoryRef;
    static ID2D1Factory*   d2d1Factory;
    static IDWriteFactory* dwriteFactory;
};