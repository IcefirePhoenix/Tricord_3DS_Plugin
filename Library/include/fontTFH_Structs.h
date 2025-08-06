#pragma once
#include <3ds/types.h>
#include <3ds.h>

// Default system font uses BCFNT whereas TFH font uses BFFNT v4; format defined here to fix parsing issues:
// https://nintendo-formats.com/libs/nw/bffnt.html
// http://www.3dbrew.org/wiki/BCFNT#Version_4_(BFFNT)

typedef struct
{
    u8 cellWidth;    // Glyph cell width
    u8 cellHeight;   // Glyph cell height
    u8 nSheets;      // Texture sheet count
    u8 maxCharWidth;

    u32 sheetSize;   // Texture sheet size (in bytes)
    u16 baselinePos;
    u16 sheetFmt;    // GPU texture format (A4)

    u16 nRows;
    u16 nLines;

    u16 sheetWidth;  //  Texture sheet width
    u16 sheetHeight; //  Texture sheet height
    u8 *sheetData;   //  Pointer to texture sheet data
} TGLP_TFH;

typedef struct
{
    u32 signature;
    u32 sectionSize;

    u8 fontType;
    u8 height;
    u8 width;
    u8 ascent;
    u16 lineFeed;
    u16 alterCharIndex; // Replacement character index; used for invalid chars
    u32 unused;

    TGLP_TFH *tglp; // Pointer to texture sheet information
    CWDH_s *cwdh;   // Pointer to the first character width information block
    CMAP_s *cmap;   // Pointer to the first character map
} FINF_TFH;

typedef struct
{
    u32 signature;
    u16 endianness; // Endianness constant (0xFEFF)
    u16 headerSize;
    u32 version;    // BFFNT v4 in this case
    u32 fileSize;
    u32 nBlocks;

    FINF_TFH finf;
} CFNT_TFH;


