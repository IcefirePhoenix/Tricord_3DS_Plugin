#include "CTRPluginFrameworkImpl/Preferences.hpp"
#include "CTRPluginFramework/System/Controller.hpp"
#include "CTRPluginFrameworkImpl/Graphics/KeyboardBG.hpp"
#include "CTRPluginFrameworkImpl/Menu/PluginMenuImpl.hpp"
#include "CTRPluginFrameworkImpl/System/Screenshot.hpp"

#include "3ds.h"
#include <cmath>

namespace CTRPluginFramework
{
    BMPImage* Preferences::bottomBackgroundImage = nullptr;
    BMPImage* Preferences::bottomBoxBGImage = nullptr;

    u32 Preferences::MenuHotkeys = static_cast<u32>(Key::Select);
    u32 Preferences::CustomNameColors[3] = { 0xFF40FF40, 0xFFFF4040, 0xFF4040FF }; // default before loading any saved values from file
    u64 Preferences::Flags = 0;

    FwkSettings Preferences::Settings;

    std::string Preferences::CheatsFile;
    std::string Preferences::ScreenshotPath;
    std::string Preferences::ScreenshotPrefix;

    bool Preferences::_cheatsAlreadyLoaded = false; // TODO: reuse for something else
    bool Preferences::_favoritesAlreadyLoaded = false;
    bool Preferences::_bmpCanBeLoaded = true;

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

    /*BMPImage *UpSampleThenCrop(BMPImage *img, int maxX, int maxY)
    {
        BMPImage *temp = UpSampleUntilItsEnough(img, maxX, maxY);

        BMPImage *res =  new BMPImage(maxX, maxY);

        u32 cx = temp->Width() / 2;
        u32 cy = temp->Height() / 2;

        temp->RoiFromCenter(cx, cy, maxX, maxY, *res);

        delete temp;

        return (res);
    }*/

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
             // Check version
            int     res = 0;

            if (settings.Read(&header, sizeof(u32) * 6)) return (-2);

            // Check file
            if (header.size != settings.GetSize()
               || !std::equal(g_signature, g_signature + 8, header.sig))
                return -1;

            if (header.version != SETTINGS_VERSION)
            {
                OSD::Notify(Color::Orange << "Config file version mismatch!");
                OSD::Notify("Default settings applied");
                return -1;
            }

            // Rewind file
            settings.Rewind();

            res = settings.Read(&header, sizeof(Header));

            return res;
        }

        return -1;
    }

    void    Preferences::LoadSettings(void)
    {
        File    settings;
        Header  header = { 0 };

        if (OpenConfigFile(settings, header) == 0)
        {
            MenuHotkeys = header.hotkeys & ((System::IsNew3DS() && Settings.AreN3DSButtonsAvailable) ? ~0x0 : ~(Key::CStick | Key::ZL | Key::ZR));
            Flags = header.flags;

            // set last saved screenshot preferences
            std::string dirPath = "/Tricord/Screenshots/";
            if (!Directory::IsExists(dirPath))
                Directory::Create(dirPath);

            dirPath.append(Process::GetRegionCode() + "/");

            if (!Directory::IsExists(dirPath))
                Directory::Create(dirPath);

            Screenshot::Path = dirPath;
            Screenshot::Prefix = std::strlen(header.screenshotCustomName) == 0 ? "Screenshot" : header.screenshotCustomName;

            // these have already been given default values under Screenshot.cpp, so only update if necessary
            if (header.screenshotHotkeys != 0)
                Screenshot::Hotkeys = header.screenshotHotkeys;

            if (header.screenshotScreenCapture != 0)
                Screenshot::Screens = header.screenshotScreenCapture; // this has already been 1-indexed when previously saved

            if (header.screenshotTimer != 0)
                Screenshot::Timer = Seconds(static_cast<float>(header.screenshotTimer));

            Screenshot::Initialize();
        }

        // Check for hotkeys to be valid
        if (MenuHotkeys == 0)
            MenuHotkeys = Key::Select;
    }

    // handles favorites, auto-enabled saved entries, hotkeys, auto-enable favs
    void Preferences::LoadEntryPreferences(bool autoEnableSavedCheats, bool autoEnableFavorites)
    {
        File settings;
        Header header = { 0 };

        if (OpenConfigFile(settings, header) == 0)
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
        }
    }

    void    Preferences::LoadBackgrounds(void)
    {
        if (!_bmpCanBeLoaded)
            return;

        std::string source = "/Tricord/Resources/Background/";

        // Try to load bottom background
        if (bottomBackgroundImage == nullptr && File::Exists(source + "CustomBottomBG.bmp"))
        {
            BMPImage* image = new BMPImage(source + "CustomBottomBG.bmp");

            if (image->IsLoaded())
                image = PostProcess(image, 320, 240);
            else
            {
                delete image;
                image = nullptr;
            }

            bottomBackgroundImage = image;
        }
        else {
            OSD::Notify("Cannot find CustomBottomBG.bmp background file!");
        }

        if (File::Exists(source + "CustomBoxBG.bmp")) {
            BMPImage* image1 = new BMPImage(source + "CustomBoxBG.bmp");

            if (image1->IsLoaded())
                image1 = PostProcess(image1, 320, 240);
            else
            {
                delete image1;
                image1 = nullptr;
            }
            bottomBoxBGImage = image1;
        }
        else {
            OSD::Notify("Cannot find CustomBoxBG.bmp background file!");
        }

        // Update Window
        Window::UpdateBackgrounds();

        _bmpCanBeLoaded = false;
    }

    void    Preferences::UnloadBackgrounds(void)
    {
        if (bottomBackgroundImage)
        {
            _bmpCanBeLoaded = true;

            delete bottomBackgroundImage;
            bottomBackgroundImage = nullptr;
        }

        // Update Window
        Window::UpdateBackgrounds();
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
            PluginMenuImpl::WriteScreenshotConfigToFile(header, settings);

            header.size = settings.Tell();
            settings.Rewind();
            settings.Write(&header, sizeof(Header));
        }

    error:

        PluginMenuActionReplay::SaveCodes();
        OSDImpl::DrawSaveIcon = false;
    }

    void    Preferences::Initialize(void)
    {
        LoadBackgrounds();
    }
}
