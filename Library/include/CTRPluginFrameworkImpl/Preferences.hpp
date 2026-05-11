#ifndef CTRPPLUGINFRAMEWORK_PREFERENCES_HPP
#define CTRPPLUGINFRAMEWORK_PREFERENCES_HPP

#include "types.h"
#include "3ds.h"
#include "CTRPluginFramework/System/FwkSettings.hpp"
#include "CTRPluginFrameworkImpl/Graphics/BMPImage.hpp"
#include <vector>

// This must be changed every time CTRPFData.bin's structure is updated!
// Ref: devkitPro/libctru/include/3ds/os.h
#define SETTINGS_VERSION SYSTEM_VERSION(1, 1, 4)
namespace CTRPluginFramework
{
    class Preferences
    {
    public:
        // Preferences::Flags = u64 = 64 bits available to use
        enum
        {
            // Settings
            HIDToggle           = 1,
            AutoSaveCheats      = 1 << 2,
            DisableOSDNotifs    = 1 << 3,
            AutoEnableSavedCheats      = 1 << 4,
            AutoEnableFavorites = 1 << 5,
            ScreenshotEnabled   = 1 << 6,

            // Misc
            DisplayLoadedFiles  = 1 << 16,
            WriteLoadedFiles    = 1 << 17,
            DrawTouchPosition   = 1 << 19,
            ShowTopFps          = 1 << 20,
            ShowBottomFps       = 1 << 21,

            // TFH Settings
            QoL_Patch           = 1 << 22,
            Prevent_Close_Menu_B        = 1 << 23
        };


        struct EnabledCheats
        {
            u32     uid;
        };

        struct Favorites
        {
            u32     uid;
        };

        struct HotkeysInfos
        {
            u32     uid;        ///< owner of the hotkeys
            u32     count;
            std::vector<u32>   hotkeys;
        };

        struct WarpDestination
        {
            u8 levelID;
            u8 stageID;
        };

        struct FaceExprFrameVal
        {
            u8 eyeVal;
            u8 mayuVal;
            u8 mouthVal;

            bool operator == (const FaceExprFrameVal &other) const
            {
                return eyeVal == other.eyeVal &&
                       mayuVal == other.mayuVal &&
                       mouthVal == other.mouthVal;
            }
        };

        struct TricordHeaderV1
        {
            u8 sig[8];
            u32 version;
            u32 pluginVersion;
            u64 size;
            u64 flags;
            u32 hotkeys;
            u32 enabledCheatsCount;
            u64 enabledCheatsOffset;
            u32 favoritesCount;
            u64 favoritesOffset;
            u32 hotkeysCount;
            u64 hotkeysOffset;
            u32 nameColor[3];
            u64 nameColorOffset;
            WarpDestination warpDestinationData[3];
            u64 warpDestOffset;
            FaceExprFrameVal customFaceExprVals[6];
            u64 customFaceExprOffset;
            u32 screenshotScreenCapture;
            u32 screenshotHotkeys;
            u32 screenshotTimer;
            char screenshotCustomName[64];
            char screenshotCustomDir[64];
            u64 screenshotOffset;
            u32 reserved[100];
        } PACKED;

        using Header = TricordHeaderV1;

        static bool         IsEnabled(u64 setting)
        {
            return (Flags & setting) > 0ULL;
        }

        static void         Set(u64 setting)
        {
            Flags |= setting;
        }

        static void         Clear(u64 setting)
        {
            Flags &= ~setting;
        }

        static void         Toggle(u64 setting)
        {
            Flags ^= setting;
        }

        static BMPImage     *bottomBackgroundImage;

        static u32          MenuHotkeys;
        static u32          CustomNameColors[3];
        static u64          Flags;
        static FwkSettings  Settings;

        static std::string  CheatsFile;
        static std::string  ScreenshotPath;
        static std::string  ScreenshotPrefix;
        static std::array<Preferences::FaceExprFrameVal, 6> SavedFaceExprs;

        static Preferences::WarpDestination SavedWarps[3];

        static int          OpenConfigFile(File &file, Header &header);
        static void         LoadSettings(void);
        static void         LoadEntryPreferences(bool autoEnableSavedCheats, bool autoEnableFavorites);
        static void         WriteSettings(void);

    private:
        static bool         _favoritesAlreadyLoaded;

        friend class PluginMenuImpl;
    };
}

#endif
