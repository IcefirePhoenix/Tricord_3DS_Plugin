#pragma once
#include <3ds/types.h>
#include "fontTFH_Structs.h"

// Retrieves TGLP, CMAP, and CWDH pointers
void fontFixTFHPointers(CFNT_TFH *font);

// Returns FINF
static inline FINF_TFH *fontGetInfo(CFNT_TFH *font)
{
    return &font->finf;
}

// Returns TGLP
static inline TGLP_TFH *fontGetGlyphInfo(CFNT_TFH *font)
{
    return fontGetInfo(font)->tglp;
}

// Returns texture sheet data given index (TFH font only has one sheet; only one index possible)
static inline void *fontGetGlyphSheetTex(CFNT_TFH *font, int sheetIndex)
{
    TGLP_TFH *tglp = fontGetGlyphInfo(font);
    return &tglp->sheetData[sheetIndex * tglp->sheetSize];
}

// Given Unicode codepoint, returns corresponding glyph index mapping
int fontTFHGlyphIndexFromCodePoint(CFNT_TFH *font, u32 codePoint);

// Given glyph index, returns info about the char's width and start position
charWidthInfo_s *fontGetTFHCharWidthInfo(CFNT_TFH *font, int glyphIndex);

/**
 * Description taken from libctru:
 * @brief Calculates position information for the specified glyph.
 * @param out Output structure in which to write the information.
 * @param font Pointer to font structure. If NULL, the shared system font is used.
 * @param glyphIndex Index of the glyph.
 * @param flags Calculation flags (see GLYPH_POS_* flags).
 * @param scaleX Scale factor to apply horizontally.
 * @param scaleY Scale factor to apply vertically.
 */
void fontTFHCalcGlyphPos(fontGlyphPos_s *out, CFNT_TFH *font, int glyphIndex, u32 flags, float scaleX, float scaleY);