#ifndef CTRPLUGINFRAMEWORKIMPL_FONT_HPP
#define CTRPLUGINFRAMEWORKIMPL_FONT_HPP

#include "types.h"
#include "CTRPluginFramework/System/Mutex.hpp"

namespace CTRPluginFramework
{
    struct Glyph
    {
        float   xOffset; ///< Horizontal offset to draw the glyph width.
        float   xAdvance; ///< Horizontal distance to advance after drawing the glyph.
        u8      *glyph; //< Glyph data to draw

        float   Width(void) const; // Return the glyph's width
    };

    class Font
    {
    public:
        static Glyph    *GetGlyph(u8* &c);
        static Glyph    *GetGlyph(char c);
        static void     Initialize(u32 fontDataStartAddr);

    private:
    static void     Initialize(void);

        friend void     Initialize(void);
        static Glyph    *CacheGlyph(u32 glyphIndex);
        static Mutex    _mutex;
    };
}

#endif
