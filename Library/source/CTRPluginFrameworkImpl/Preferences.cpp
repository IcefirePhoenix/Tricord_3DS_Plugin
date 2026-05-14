#include "CTRPluginFrameworkImpl/Preferences.hpp"
#include "CTRPluginFramework/System/Controller.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuImpl.hpp"
#include "CTRPluginFrameworkImpl/System/Screenshot.hpp"
#include "3ds.h"
#include <cmath>

namespace CTRPluginFramework
{
    enum ConfigStatus
    {
        CONFIG_OK = 0,
        CONFIG_MISSING = -1,
        CONFIG_INVALID_HEADER = -2,
        CONFIG_INVALID_SIG = -3,
        CONFIG_OUTDATED = -4,
        CONFIG_METADATA_INVALID = -5,
        CONFIG_BODY_INVALID = -6
    };

    BMPImage* Preferences::bottomBackgroundImage = nullptr;

    u32 Preferences::MenuHotkeys = static_cast<u32>(Key::Select);
    u32 Preferences::CustomNameColors[3] = { 0xFF40FF40, 0xFFFF4040, 0xFF4040FF }; // default before loading any saved values from file
    u64 Preferences::Flags = 0;

    FwkSettings Preferences::Settings;

    std::string Preferences::CheatsFile;
    std::string Preferences::ScreenshotPath;
    std::string Preferences::ScreenshotPrefix;

    std::array<Preferences::FaceExprFrameVal, 6> Preferences::SavedFaceExprs = // default before loading any saved values from file
    {{
        {0, 0, 0},
        {8, 2, 1},
        {6, 1, 2},
        {0, 1, 1},
        {6, 3, 1},
        {4, 1, 1}
    }};

    bool Preferences::_favoritesAlreadyLoaded = false;

    Preferences::WarpDestination Preferences::SavedWarps[3];

    static const char *g_signature = "CTRPF\0\0";

    BMPImage *RegionFromCenter(BMPImage *img, int maxX, int maxY)
    {
        BMPImage *temp = new BMPImage(maxX, maxY);

        u32 cx = img->Width() / 2;
        u32 cy = img->Height() / 2;

        img->RoiFromCenter(cx, cy, maxX, maxY, *temp);

        delete img;
        return (temp);
    }

    BMPImage *UpSampleUntilItsEnough(BMPImage *img, int maxX, int maxY)
    {
        BMPImage *temp = new BMPImage(img->Width() * 2, img->Height() * 2);

        img->UpSample(*temp);
        delete img;

        if (temp->Width() < static_cast<u32>(maxX) || temp->Height() < static_cast<u32>(maxY))
            return (UpSampleUntilItsEnough(temp, maxX, maxY));
        return (temp);
    }

    float GetRatio(int width, int height, int maxX, int maxY)
    {
        if (width >= height)
            return ((float)width / maxX);
        return ((float)height / maxY);
    }

    BMPImage *PostProcess(BMPImage *img, int maxX, int maxY)
    {
        int width = img->Width();
        int height = img->Height();

        if (width == maxX && height == maxY)
            return img;

        float ratio = GetRatio(width, height, maxX, maxY);

        int newWidth = (int)(ceilf((float)width / ratio));
        int newHeight = (int)(ceilf((float)height / ratio));

        BMPImage *temp = new BMPImage(newWidth, newHeight);

        img->Resample(*temp, newWidth, newHeight);
        delete img;

        if (newWidth != maxX || newHeight != maxY)
        {
            BMPImage *res = new BMPImage(*temp, maxX, maxY);
            delete temp;
            return res;
        }

        return (temp);
    }

    int     Preferences::OpenConfigFile(File &settings, Header &header)
    {
        if (File::Open(settings, "CTRPFData.bin") == 0 && settings.GetSize() > 0)
        {
            if (settings.Read(&header, sizeof(u32) * 6))
            {
                return ConfigStatus::CONFIG_METADATA_INVALID;
            }

            if (header.size != settings.GetSize())
            {
                return ConfigStatus::CONFIG_INVALID_HEADER;
            }

            if (!std::equal(g_signature, g_signature + 8, header.sig))
            {
                return ConfigStatus::CONFIG_INVALID_SIG;
            }

            if (header.version != SETTINGS_VERSION)
            {
                return ConfigStatus::CONFIG_OUTDATED;
            }

            settings.Rewind();

            if (settings.Read(&header, sizeof(Header)) == 0)
            {
                return ConfigStatus::CONFIG_OK;
            }
            else
            {
                return ConfigStatus::CONFIG_BODY_INVALID;
            }
        }
        return ConfigStatus::CONFIG_MISSING;
    }

    void    Preferences::LoadSettings(void)
    {
        File settings;
        Header header = { 0 };

        std::string dirPath = "/Tricord/Screenshots/";
        if (!Directory::IsExists(dirPath))
            Directory::Create(dirPath);

        dirPath.append(Process::GetRegionCode() + "/");

        if (!Directory::IsExists(dirPath))
            Directory::Create(dirPath);

        Screenshot::Path = dirPath;
        Screenshot::Prefix = "Screenshot";

        int configReadStatus = OpenConfigFile(settings, header);
        switch (configReadStatus)
        {
            case ConfigStatus::CONFIG_OK:
            {
                MenuHotkeys = header.hotkeys & ((System::IsNew3DS() && Settings.AreN3DSButtonsAvailable) ? ~0x0 : ~(Key::CStick | Key::ZL | Key::ZR));
                Flags = header.flags;

                Screenshot::Prefix = std::strlen(header.screenshotCustomName) == 0 ? "Screenshot" : header.screenshotCustomName;

                // these have already been given default values under Screenshot.cpp, so only update if necessary
                if (header.screenshotHotkeys != 0)
                    Screenshot::Hotkeys = header.screenshotHotkeys;

                if (header.screenshotScreenCapture != 0)
                    Screenshot::Screens = header.screenshotScreenCapture; // this has already been 1-indexed when previously saved

                if (header.screenshotTimer != 0)
                    Screenshot::Timer = Seconds(static_cast<float>(header.screenshotTimer));

                break;
            }
            case ConfigStatus::CONFIG_OUTDATED:
                OSD::Notify("Outdated config detected, reverting to default settings.", Color::Orange);
                Preferences::Set(Preferences::QoL_Patch);
                break;

            default:
                OSD::Notify("Config file invalid or missing, reverting to default settings.", Color::Orange);
                Preferences::Set(Preferences::QoL_Patch);
                break;
        }

        Screenshot::Initialize();

        // Check for hotkeys to be valid
        if (MenuHotkeys == 0)
            MenuHotkeys = Key::Select;
    }

    // handles favorites, auto-enabled saved entries, hotkeys, auto-enable favs
    void Preferences::LoadEntryPreferences(bool autoEnableSavedCheats, bool autoEnableFavorites)
    {
        File settings;
        Header header = { 0 };

        int configReadStatus = OpenConfigFile(settings, header);
        switch (configReadStatus)
        {
            case ConfigStatus::CONFIG_OK:
            {
                if (header.favoritesCount != 0)
                {
                    PluginMenuImpl::LoadFavoritesFromFile(header, settings);

                    if (autoEnableFavorites)
                        PluginMenuImpl::ActivateFavoritesFromFile(header, settings);
                }

                if (header.hotkeysCount != 0)
                    PluginMenuImpl::LoadHotkeysFromFile(header, settings);

                if (autoEnableSavedCheats && header.enabledCheatsCount != 0)
                    PluginMenuImpl::ActivateEnabledCheatsFromFile(header, settings);

                PluginMenuImpl::LoadNameColorsFromFile(header, settings);
                PluginMenuImpl::LoadBookmarkWarpsFromFile(header, settings);
                PluginMenuImpl::LoadFaceExprFromFile(header, settings);

                break;
            }
            default:
                OSD::Notify("Saved user preferences have been reset.", Color::Orange);
                Preferences::Set(Preferences::QoL_Patch);
                break;
        }
    }

    void    Preferences::WriteSettings(void)
    {
        OSDImpl::DrawSaveIcon = true;

        File    settings;
        int     mode = File::READ | File::WRITE | File::CREATE | File::TRUNCATE | File::SYNC;
        Header  header = { 0 };

        std::copy(g_signature, g_signature + 8, header.sig);
        header.version = SETTINGS_VERSION;
        header.hotkeys = MenuHotkeys;
        header.flags = Flags;

        if (File::Open(settings, "CTRPFData.bin", mode) == 0)
        {
            if (settings.Write(&header, sizeof(Header)) != 0) goto error;

            if (IsEnabled(AutoSaveCheats))
                PluginMenuExecuteLoop::WriteEnabledCheatsToFile(header, settings);

            PluginMenuImpl::WriteFavoritesToFile(header, settings);
            PluginMenuImpl::WriteHotkeysToFile(header, settings);

            // save name colors and warp locations...
            PluginMenuImpl::WriteCustomNameColorToFile(header, settings);
            PluginMenuImpl::WriteBookmarkWarpsToFile(header, settings);
            PluginMenuImpl::WriteFaceExprsToFile(header, settings);
            PluginMenuImpl::WriteScreenshotConfigToFile(header, settings);

            header.size = settings.Tell();
            settings.Rewind();
            settings.Write(&header, sizeof(Header));
        }

    error:

        PluginMenuActionReplay::SaveCodes();
        OSDImpl::DrawSaveIcon = false;
    }
}
