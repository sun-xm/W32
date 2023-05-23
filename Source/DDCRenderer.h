#pragma once

#include "D2DRenderer.h"

class D2DDc
{
public:
    D2DDc();
    D2DDc(int width, int height);
    D2DDc(D2DDc&& other);
    D2DDc(const D2DDc&) = delete;
   ~D2DDc();

    operator bool() const;
    operator HDC() const;

    D2DDc& operator=(D2DDc&& other);
    D2DDc& operator=(const D2DDc&) = delete;

    int Width() const;
    int Height() const;

private:
    int w, h;

    HDC     hdc;
    HBITMAP bmp;
    HGDIOBJ old;
};

class DDCRenderer : public D2DRenderer
{
public:
    static DDCRenderer Create();

    DDCRenderer() = default;
    DDCRenderer(const DDCRenderer&);
    DDCRenderer(ID2D1DCRenderTarget*);

    bool ResizeTarget(int width, int height);

    HDC GetDC() const;

    DDCRenderer& operator=(const DDCRenderer&);

private:
    D2DDc dc;
};