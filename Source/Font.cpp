#include "Font.h"

Font::Font() : font(0)
{
}

Font::Font(Font&& other) : Font()
{
    auto font  = this->font;
    this->font = other.font;
    other.font = font;
}

Font::~Font()
{
    if (this->font)
    {
        DeleteObject(this->font);
    }
}

Font::operator HFONT() const
{
    return this->font;
}

Font Font::Create(const wchar_t* family, int size, int weight, bool italic, bool underline, bool strikeOut, DWORD charSet, DWORD outPrecision, DWORD clipPrecision, DWORD quality, DWORD pitchAndFamity, int escapement, int orientation)
{
    auto f = CreateFontW(size, 0, escapement, orientation, weight, italic ? TRUE : FALSE, underline ? TRUE : FALSE, strikeOut ? TRUE : FALSE, charSet, outPrecision, clipPrecision, quality, pitchAndFamity, family);

    Font font;
    font.font = f;
    return font;
}