#pragma once

#include <Windows.h>

class Font
{
public:
    Font();
    Font(Font&&);
    Font(const Font&) = delete;
   ~Font();

    Font& operator=(Font&&) = delete;
    Font& operator=(const Font&) = delete;

    operator HFONT() const;

    static Font Create(const wchar_t* family, int size, int weight = FW_DONTCARE, bool italic = false, bool underline = false, bool strikeOut = false, DWORD charSet = DEFAULT_CHARSET, DWORD outPrecision = OUT_DEFAULT_PRECIS, DWORD clipPrecision = CLIP_DEFAULT_PRECIS, DWORD quality = DEFAULT_QUALITY, DWORD pitchAndFamity = DEFAULT_PITCH | FF_DONTCARE, int escapement = 0, int orientation = 0);

protected:
    HFONT font;
};