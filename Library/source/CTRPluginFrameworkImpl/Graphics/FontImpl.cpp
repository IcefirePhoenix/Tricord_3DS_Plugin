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
#include <unordered_map>

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
        std::unordered_map<u32, u32> defaultSysFont;
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

    inline u8    GetAlphaValueFromData(u8* data, int dataPos, u16 format) {
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
        return (glyph);
    }

#define GLYPH_HEIGHT    25
#define GLYPH_WIDTH     19
#define SHRINK_GLYPH_HEIGHT 18 // (width would be 14)

    void    ShrinkGlyph(u8 *dest, u8 *src)
    {
        constexpr const int  outWidth = std::round(static_cast<float>(GLYPH_WIDTH)
            * (static_cast<float>(SHRINK_GLYPH_HEIGHT) / static_cast<float>(GLYPH_HEIGHT)));
        constexpr const float dx = (static_cast<float>(GLYPH_WIDTH) / outWidth);
        constexpr const float dy = (static_cast<float>(GLYPH_HEIGHT) / static_cast<float>(SHRINK_GLYPH_HEIGHT));

        int i, ii = 0;

        for (float yt = 0.f, y = 0.f; y < SHRINK_GLYPH_HEIGHT; y++, yt += dy)
        {
            float yfrag = ceil(yt) - yt;

            if (yfrag == 0.f)
                yfrag = 1.f;
            float yfrag2 = yt+dy - floor(yt + dy);

            if(yfrag2 == 0.f && dy != 1.0f)
                yfrag2 = 1.f;

            for (float xt = 0.f, x = 0.f; x < outWidth; x++, xt += dx)
            {
                int xi = static_cast<int>(xt);
                int yi = static_cast<int>(yt);
                float xfrag = ceil(xt) - xt;
                if(xfrag == 0.f)
                    xfrag = 1.f;
                float xfrag2 = xt + dx - floor(xt+dx);
                if(xfrag2 == 0.f && dx != 1.0f)
                    xfrag2 = 1.f;
                float alpha = xfrag * yfrag * src[(yi * GLYPH_WIDTH + xi)];

                for(i=0; xi + i + 1 < xt+dx-1; i++)
                    alpha += yfrag * src[(yi * GLYPH_WIDTH + xi + i + 1)];

                alpha += xfrag2 * yfrag * src[(yi*GLYPH_WIDTH +xi+i+1)];

                for(i = 0; yi + i + 1 < yt + dy - 1 && yi + i + 1 < GLYPH_HEIGHT; i++)
                {
                    alpha += xfrag * src[((yi + i + 1) * GLYPH_WIDTH + xi)];

                    for (ii = 0; xi + ii + 1 < xt + dx - 1 && xi + ii + 1 < GLYPH_WIDTH; ii++)
                        alpha += src[((yi + i + 1) * GLYPH_WIDTH + xi + ii + 1)];

                    if (yi + i + 1 < GLYPH_WIDTH && xi + ii + 1 < GLYPH_WIDTH)
                        alpha += xfrag2 * src[((yi + i + 1) * GLYPH_WIDTH + xi + ii + 1)];
                }

                if (yi + i + 1 < GLYPH_HEIGHT)
                {
                    alpha += xfrag * yfrag2 * src[((yi + i + 1) * GLYPH_WIDTH + xi)];

                    for (ii = 0; xi + ii + 1 < xt + dx - 1 && xi + ii + 1 < GLYPH_WIDTH; ii++)
                        alpha += yfrag2 * src[((yi + i + 1) * GLYPH_WIDTH + xi + ii + 1)];
                }

                if (yi + i + 1 < GLYPH_HEIGHT && xi + ii + 1 < GLYPH_WIDTH)
                    alpha += xfrag2 * yfrag2 * src[((yi + i + 1) * GLYPH_WIDTH + xi + ii + 1)];

                alpha /= dx * dy;
                alpha = alpha <= 0.f ? 0.f : (alpha >= 255.f ? 255.f : alpha);
                dest[(static_cast<u32>(y) * outWidth + static_cast<u32>(x))] = static_cast<u8>(alpha);
            }
        }
    }

    Glyph   *Font::CacheGlyph(u32 glyphIndex)
    {
        // Check if the glyph already exists
        {
            Glyph *glyph = reinterpret_cast<Glyph *>(defaultSysFont[glyphIndex]);
            if (glyph != nullptr)
                return glyph;
        }

        Lock    lock(_mutex);

        u8  *originalGlyph = GetOriginalGlyph(glyphIndex);
        // 14x18 wxh = 252 pixels (A4 image format)
        u8 *newGlyph = new u8[252];
        g_fontAllocated += 252;
        std::memset(newGlyph, 0, 252);

        // Shrink glyph data to the required size
        ShrinkGlyph(newGlyph, originalGlyph);

        // Allocate new Glyph
        Glyph *glyph = new Glyph; //static_cast<Glyph *>(linearAlloc(sizeof(Glyph)));
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

        // Add Glyph to defaultSysFont
        defaultSysFont[glyphIndex] = reinterpret_cast<u32>(glyph);

        return (glyph);
    }
}
