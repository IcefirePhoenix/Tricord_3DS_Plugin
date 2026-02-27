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

#define OG_GLYPH_WIDTH 19
#define OG_GLYPH_HEIGHT 25
#define NEW_GLYPH_WIDTH 14
#define NEW_GLYPH_HEIGHT 18
#define SHEET_HEIGHT 1024

namespace CTRPluginFramework
{
    extern "C" CFNT_TFH* TFH_Font;
    extern "C" int g_charPerSheet;

    int maxGlyphs = 739;
    int sheetWidth = 512;

    Mutex Font::_mutex;

    namespace
    {
        u8 *glyphBuffer = nullptr;
        u8 *rowBuffer = nullptr;
        Glyph *defaultGlyph = nullptr;

        std::vector<Glyph> g_glyphPool;
        std::vector<u8> g_bitmapPool;

        static std::vector<Glyph*> glyphCache;
    }

    float Glyph::Width(void) const
    {
        return (xOffset + xAdvance);
    }

    static void LocateTFH_MessageFontBFFNT(u32 fontDataStart)
    {
        TFH_Font = reinterpret_cast<CFNT_TFH*>(fontDataStart);
        fontFixTFHPointers(TFH_Font);
        g_charPerSheet = TFH_Font->finf.tglp->nRows * TFH_Font->finf.tglp->nLines; // TFH font only has one sheet, so this is effectively the total glyph count
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

    // Modeled after parser code by ObsidianX
    // https://github.com/ObsidianX/3dstools/blob/master/bffnt.py
    void DeswizzleSheets(void)
    {
        const char *dirpath = "/Tricord/linearSheet.tmp";

        TGLP_TFH *tglp = fontGetGlyphInfo(TFH_Font);
        u8 *data = (u8 *)fontGetGlyphSheetTex(TFH_Font, 0);

        if (!File::Exists(dirpath))
            File::Create(dirpath);

        File fontFile;
        File::Open(fontFile, dirpath, File::WRITE | File::CREATE | File::TRUNCATE);

        int tileWidth = sheetWidth / 8;
        int tileHeight = SHEET_HEIGHT / 8;

        int batchHeight = 256;
        int tileRowsPerBatch = batchHeight / 8;

        int totalBatches = (tileHeight + tileRowsPerBatch - 1) / tileRowsPerBatch;

        u8 *tmpBuffer = new u8[sheetWidth * batchHeight];

        for (int batch = 0; batch < totalBatches; batch++)
        {
            int batchStartTileY = batch * tileRowsPerBatch;
            int batchEndTileY = std::min(batchStartTileY + tileRowsPerBatch, tileHeight);

            std::memset(tmpBuffer, 0, sheetWidth * batchHeight);

            for (int tileY = batchStartTileY; tileY < batchEndTileY; tileY++)
            {
                for (int tileX = 0; tileX < tileWidth; tileX++)
                {
                    for (int y = 0; y < 2; y++)
                    {
                        for (int x = 0; x < 2; x++)
                        {
                            for (int yy = 0; yy < 2; yy++)
                            {
                                for (int xx = 0; xx < 2; xx++)
                                {
                                    for (int yyy = 0; yyy < 2; yyy++)
                                    {
                                        for (int xxx = 0; xxx < 2; xxx++)
                                        {
                                            int pixelX = (xxx + (xx * 2) + (x * 4) + (tileX * 8));
                                            int pixelY = (yyy + (yy * 2) + (y * 4) + (tileY * 8));

                                            if (pixelX >= sheetWidth || pixelY >= SHEET_HEIGHT)
                                                continue;

                                            int dataX = (xxx + (xx * 4) + (x * 16) + (tileX * 64));
                                            int dataY = ((yyy * 2) + (yy * 8) + (y * 32) + (tileY * sheetWidth * 8));
                                            int dataPos = dataX + dataY;

                                            int localPixelY = pixelY - (batchStartTileY * 8);
                                            tmpBuffer[pixelX + localPixelY * sheetWidth] = GetAlphaValueFromData(data, dataPos, tglp->sheetFmt);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            int batchStartPixelY = batchStartTileY * 8;

            fontFile.Seek(batchStartPixelY * sheetWidth, File::SET);
            fontFile.Write(tmpBuffer, sheetWidth * batchHeight);
        }

        fontFile.Close();
        delete[] tmpBuffer;
    }

    void Font::Initialize(u32 fontDataStart, bool useDoubleWidth)
    {
        File fontFile;
        bool done = false;

        if (useDoubleWidth)
        {
            sheetWidth *= 2;
            maxGlyphs = 1815;
        }

        LocateTFH_MessageFontBFFNT(fontDataStart);

        TGLP_TFH *tglp = fontGetGlyphInfo(TFH_Font); // relies on line above!
        int glyphsPerRow = tglp->nRows;

        glyphBuffer = (u8 *)new u8[OG_GLYPH_WIDTH * OG_GLYPH_HEIGHT]; // holds pixel data belonging to a glyph
        rowBuffer = (u8 *)new u8[sheetWidth * OG_GLYPH_HEIGHT];

        g_glyphPool.resize(maxGlyphs);
        g_bitmapPool.resize(maxGlyphs * NEW_GLYPH_WIDTH * NEW_GLYPH_HEIGHT);

        glyphCache.clear();
        glyphCache.resize(maxGlyphs, nullptr);

        DeswizzleSheets();

        File::Open(fontFile, "/Tricord/linearSheet.tmp", File::READ);

        for (int processedRows = 0; processedRows < (int)tglp->nLines && !done; processedRows++)
        {
            std::memset(rowBuffer, 0, sheetWidth * OG_GLYPH_HEIGHT);

            fontFile.Seek(processedRows * (sheetWidth * OG_GLYPH_HEIGHT), File::SET);
            fontFile.Read(rowBuffer, sheetWidth * OG_GLYPH_HEIGHT);

            for (int glyphIndex = 0; glyphIndex < glyphsPerRow; glyphIndex++)
            {
                if (glyphIndex + (glyphsPerRow * processedRows) >= maxGlyphs)
                {
                    done = true;
                    break;
                }

                CacheGlyph(glyphIndex + (glyphsPerRow * processedRows)); // Will create glyphCache entry
            }
        }

        fontFile.Close();
        defaultGlyph = CacheGlyph(fontTFHGlyphIndexFromCodePoint(TFH_Font, (u32)'?'));
        delete[] rowBuffer;
    }

    Glyph *Font::GetGlyph(u8* &c)
    {
        u32 code;
        u32 glyphIndex;
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
                return defaultGlyph;
            }

            // Retrieve from cache
            if (glyphIndex < glyphCache.size() && glyphCache[glyphIndex] != nullptr)
                return glyphCache[glyphIndex];
        }
        return nullptr;
    }

    Glyph *Font::GetGlyph(char c)
    {
        u8 *s = (u8 *)&c;
        return GetGlyph(s);
    }

    u8 *GetOriginalGlyph(u32 glyphIndex)
    {
        TGLP_TFH *tglp = fontGetGlyphInfo(TFH_Font);

        int glyphsPerRow = tglp->nRows;
        int indexX = glyphIndex % glyphsPerRow;
        int startPosX = indexX * OG_GLYPH_WIDTH;

        std::memset(glyphBuffer, 0, OG_GLYPH_WIDTH * OG_GLYPH_HEIGHT);

        for (int y = 0; y < OG_GLYPH_HEIGHT; y++)
        {
            for (int x = 0; x < OG_GLYPH_WIDTH; x++)
            {
                glyphBuffer[x + y * OG_GLYPH_WIDTH] = rowBuffer[(startPosX + x) + y * sheetWidth];
            }
        }

        return glyphBuffer;
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

    Glyph* Font::CacheGlyph(u32 glyphIndex)
    {
        if (glyphCache[glyphIndex] != nullptr) // if Glyph was already cached, return it
            return glyphCache[glyphIndex];

        Lock lock(_mutex);

        u8 *originalGlyph = GetOriginalGlyph(glyphIndex);
        u8 *newResizedGlyph = g_bitmapPool.data() + (glyphIndex * NEW_GLYPH_WIDTH * NEW_GLYPH_HEIGHT);

        std::memset(newResizedGlyph, 0, NEW_GLYPH_WIDTH * NEW_GLYPH_HEIGHT);

        ShrinkGlyph(newResizedGlyph, originalGlyph, OG_GLYPH_WIDTH, OG_GLYPH_HEIGHT, NEW_GLYPH_WIDTH, NEW_GLYPH_HEIGHT);

        Glyph *glyph = &g_glyphPool[glyphIndex];

        std::memset(glyph, 0, sizeof(Glyph));

        // Get Glyph data
        charWidthInfo_s *cwi;
        fontGlyphPos_s glyphPos;
        Renderer::FontCalcGlyphPos(&glyphPos, &cwi, glyphIndex, 14.0f / 19.0f);

        glyph->xOffset = std::floor((glyphIndex == 0) ? 0 : glyphPos.xOffset);
        glyph->xAdvance = std::round((glyphIndex == 0) ? glyphPos.xAdvance : (glyphPos.xAdvance - glyphPos.xOffset));
        glyph->glyph = newResizedGlyph;

        // Add to cache
        if (glyphIndex < glyphCache.size())
            glyphCache[glyphIndex] = glyph;

        return glyph;
    }
}
