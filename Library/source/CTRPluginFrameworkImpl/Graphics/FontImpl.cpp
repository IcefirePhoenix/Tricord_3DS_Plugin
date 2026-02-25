#include "3ds.h"
#include "CTRPluginFramework/Graphics/CustomFont.hpp"
#include "CTRPluginFramework/Utils/Utils.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Font.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Renderer.hpp"
#include "CTRPluginFramework/System/Lock.hpp"
#include "CTRPluginFrameworkImpl/Search/Search.hpp"

#include "ctrulibExtension.h"
#include "fontTFH_Structs.h"

#include <cstring>
#include <cmath>
#include <vector>

namespace CTRPluginFramework
{
    extern "C" CFNT_TFH* TFH_Font;
    extern "C" int g_charPerSheet;

    u32     g_fontAllocated = 0;
    u32     g_glyphAllocated = 0;
    Mutex  Font::_mutex;

    namespace
    {
        u8 *glyph = nullptr;
        Glyph *defaultGlyph = nullptr;

        constexpr size_t MAX_GLYPHS = 0x800;

        static Glyph g_glyphPool[MAX_GLYPHS];
        static bool g_glyphUsed[MAX_GLYPHS] = {false};
        static u8 g_bitmapPool[MAX_GLYPHS][252];

        static std::vector<Glyph*> glyphCache;

        static Glyph* AllocateGlyph(u32 glyphIndex)
        {
            if (glyphIndex >= MAX_GLYPHS)
                return nullptr;

            if (!g_glyphUsed[glyphIndex])
            {
                g_glyphUsed[glyphIndex] = true;
                return &g_glyphPool[glyphIndex];
            }
            return nullptr;
        }
    }

    float Glyph::Width(void) const
    {
        return (xOffset + xAdvance);
    }

    static void LocateTFH_MessageFontBFFNT(u32 fontDataStart)
    {
        TFH_Font = reinterpret_cast<CFNT_TFH*>(fontDataStart);
        fontFixTFHPointers(TFH_Font);
        g_charPerSheet = TFH_Font->finf.tglp->nRows * TFH_Font->finf.tglp->nLines;
    }

    void    Font::Initialize(u32 fontDataStart)
    {
        LocateTFH_MessageFontBFFNT(fontDataStart);
        glyph = (u8 *)new u8[1000];

        glyphCache.clear();
        glyphCache.resize(MAX_GLYPHS, nullptr);
    }

    Glyph   *Font::GetGlyph(u8* &c)
    {
        u32     code;
        u32     glyphIndex;
        ssize_t units;

        units = decode_utf8(&code, c);
        if (units == -1)
            return (nullptr);

        c += units;
        if (code > 0)
        {
            glyphIndex = fontTFHGlyphIndexFromCodePoint(TFH_Font, code);
            if (glyphIndex == 0xFFFF) // Glyph not found, return "?" instead
            {
                if (defaultGlyph == nullptr)
                    defaultGlyph = CacheGlyph(fontTFHGlyphIndexFromCodePoint(TFH_Font, (u32)'?'));

                return defaultGlyph;
            }

            return CacheGlyph(glyphIndex);
        }
        return (nullptr);
    }

    Glyph   *Font::GetGlyph(char c)
    {
        u8 *s = (u8 *)&c;

        return (GetGlyph(s));
    }

    inline u8 GetAlphaValueFromData(u8 *data, int dataPos, u16 format)
    {
        u8 res, byte;
        switch (format)
        {
        case GPU_A4:
            byte = data[dataPos / 2];
            res = ((byte >> ((dataPos & 1) * 4)) & 0x0F) * 0x11;
            break;
        case GPU_A8:
            res = data[dataPos];
            break;
        default: // The rest of the formats are not normally used with fonts
            res = 0;
            break;
        }
        return res;
    }

    // Original code by ObsidianX
    // https://github.com/ObsidianX/3dstools/blob/master/bffnt.py
    u8    *GetOriginalGlyph(u32 glyphIndex)
    {
        TGLP_TFH *tglp = fontGetGlyphInfo(TFH_Font);
        u8 *data = (u8 *)fontGetGlyphSheetTex(TFH_Font, glyphIndex / g_charPerSheet);

        int     width = tglp->sheetWidth; // 512
        int     height = tglp->sheetHeight; // 1024

        int     dataWidth = width;
        int     dataHeight = height;

        int     index = glyphIndex % g_charPerSheet;

        int tileWidth = width / 8; // 64
        int tileHeight = height / 8; // 128

        std::memset(glyph, 0, 1000);

        // Get the part we're interested in
        int glyphsPerRow = tglp->nRows; // 26
        int indexX = index % glyphsPerRow;
        int indexY = index / glyphsPerRow;

        int singleWx = 19;
        int singleHy = 25;
        int startPx = std::round(indexX * singleWx) - 1;
        int endPx = startPx + singleWx;
        int startPy = std::round(indexY * singleHy);
        int endPy = startPy + singleHy;

        // Sheet is composed of 8x8 pixel tiles
        for (int tileY = 0; tileY < tileHeight; tileY++)
        {
            for (int tileX = 0; tileX < tileWidth; tileX++)
            {
                // Tile is composed of 2x2 sub-tiles
                for (int y = 0; y < 2; y++)
                {
                    for (int x = 0; x < 2; x++)
                    {
                        // Subtile is composed of 2x2 pixel groups
                        for (int yy = 0; yy < 2; yy++)
                        {
                            for (int xx = 0; xx < 2; xx++)
                            {
                                // Pixel group is composed of 2x2 pixels
                                for (int yyy = 0; yyy < 2; yyy++)
                                {
                                    for (int xxx = 0; xxx < 2; xxx++)
                                    {
                                        // If the final y value is beyond the input data's height then don't read it
                                        if (tileY + y + yy + yyy >= dataHeight)
                                            continue;
                                        // Same for the x and the input data width
                                        if (tileX + x + xx + xxx >= dataWidth)
                                            continue;

                                        int pixelX = (xxx + (xx * 2) + (x * 4) + ((tileX) * 8));
                                        int pixelY = (yyy + (yy * 2) + (y * 4) + (tileY * 8));

                                        int dataX = (xxx + (xx * 4) + (x * 16) + (tileX * 64));
                                        int dataY = ((yyy * 2) + (yy * 8) + (y * 32) + (tileY * width * 8));

                                        int dataPos = dataX + dataY;

                                        if (pixelY >= startPy && pixelY < endPy)
                                            if (pixelX >= startPx && pixelX < endPx)
                                            {
                                                u32 offset = ((pixelX - startPx) + (pixelY - startPy) * (endPx - startPx));
                                                if (offset < 1000)
                                                    *(glyph + offset) = GetAlphaValueFromData(data, dataPos, tglp->sheetFmt);
                                            }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    u8 BilinearInterpolate(const u8 *bitmap, int srcWidth, int srcHeight, float x, float y)
    {
        int x1 = static_cast<int>(std::floor(x));
        int y1 = static_cast<int>(std::floor(y));
        int x2 = std::min(x1 + 1, srcWidth - 1);
        int y2 = std::min(y1 + 1, srcHeight - 1);

        float dx = x - x1;
        float dy = y - y1;

        u8 p11 = bitmap[y1 * srcWidth + x1];
        u8 p12 = bitmap[y2 * srcWidth + x1];
        u8 p21 = bitmap[y1 * srcWidth + x2];
        u8 p22 = bitmap[y2 * srcWidth + x2];

        return static_cast<u8>((1 - dx) * (1 - dy) * p11 + dx * (1 - dy) * p21 + (1 - dx) * dy * p12 + dx * dy * p22);
    }

    void ShrinkGlyph(u8 *dest, const u8 *src, int srcWidth, int srcHeight, int destWidth, int destHeight)
    {
        float scaleX = static_cast<float>(srcWidth) / destWidth;
        float scaleY = static_cast<float>(srcHeight) / destHeight;

        for (int y = 0; y < destHeight; ++y)
        {
            for (int x = 0; x < destWidth; ++x)
            {
                float srcX = x * scaleX;
                float srcY = y * scaleY;

                dest[y * destWidth + x] = BilinearInterpolate(src, srcWidth, srcHeight, srcX, srcY);
            }
        }
    }

    Glyph   *Font::CacheGlyph(u32 glyphIndex)
    {
        // Check cache first
        if (glyphIndex < glyphCache.size() && glyphCache[glyphIndex] != nullptr)
            return glyphCache[glyphIndex];

        Lock lock(_mutex);

        u8 *originalGlyph = GetOriginalGlyph(glyphIndex);

        u8 *newGlyph = g_bitmapPool[glyphIndex];
        g_fontAllocated += 252; // 14x18 wxh = 252 pixels (A4 image format)
        std::memset(newGlyph, 0, 252);

        ShrinkGlyph(newGlyph, originalGlyph);

        // Allocate Glyph from pool
        Glyph *glyph = AllocateGlyph(glyphIndex);
        if (glyph == nullptr)
            return nullptr;

        g_fontAllocated += sizeof(Glyph);
        g_glyphAllocated++;
        std::memset(glyph, 0, sizeof(Glyph));

        // Get Glyph data
        charWidthInfo_s     *cwi;
        fontGlyphPos_s      glyphPos;
        Renderer::FontCalcGlyphPos(&glyphPos, &cwi, glyphIndex, 0.75f, 0.75f);

        glyph->xOffset =  std::round((glyphIndex == 0) ? 0 : glyphPos.xOffset);
        glyph->xAdvance = std::floor((glyphIndex == 0) ? glyphPos.xAdvance : (glyphPos.xAdvance - glyphPos.xOffset));
        glyph->glyph = newGlyph;

        // Add to cache
        if (glyphIndex < glyphCache.size())
            glyphCache[glyphIndex] = glyph;

        return (glyph);
    }
}
