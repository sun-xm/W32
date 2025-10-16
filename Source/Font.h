#pragma once

#include <Windows.h>

class Font
{
public:
    Font() : font(0) {}
    Font(Font&& other) : Font()
    {
        auto font  = this->font;
        this->font = other.font;
        other.font = font;
    }
    Font(const Font&) = delete;
   ~Font()
    {
        if (this->font)
        {
            DeleteObject(this->font);
        }
    }

    Font& operator=(Font&&) = delete;
    Font& operator=(const Font&) = delete;

    operator HFONT() const
    {
        return this->font;
    }

    static Font Create(const wchar_t* family, int size, int weight = FW_DONTCARE, bool italic = false, bool underline = false, bool strikeOut = false, DWORD charSet = DEFAULT_CHARSET, DWORD outPrecision = OUT_DEFAULT_PRECIS, DWORD clipPrecision = CLIP_DEFAULT_PRECIS, DWORD quality = DEFAULT_QUALITY, DWORD pitchAndFamity = DEFAULT_PITCH | FF_DONTCARE, int escapement = 0, int orientation = 0)
    {
        auto f = CreateFontW(size, 0, escapement, orientation, weight, italic ? TRUE : FALSE, underline ? TRUE : FALSE, strikeOut ? TRUE : FALSE, charSet, outPrecision, clipPrecision, quality, pitchAndFamity, family);

        Font font;
        font.font = f;
        return font;
    }

protected:
    HFONT font;
};