#ifndef CTRPLUGINFRAMEWORK_ICON_HPP
#define CTRPLUGINFRAMEWORK_ICON_HPP

#include "CTRPluginFramework/System/Vector.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuEntryTools.hpp"

namespace CTRPluginFramework
{
    class Icon
    {
    public:
        enum Rotation
        {
            ROT_0,
            ROT_90,
            ROT_180,
            ROT_270
        };

        /*
        ** 3DS
        ** 25px * 25px
        ************/
        static int Draw3DS(int posX, int posY, bool enabled);

        /*
        ** About
        ** 15px * 15px
        ************/
        static int DrawAbout(int posX, int posY);

        /*
        ** BG_Underline
        ** 15px * 15px
        ************/
        static int DrawBG_Underline(int posX, int posY);

        /*
        ** MenuButton
        ** 32px * 120px
        ************/
        static int DrawMenuButton(int posX, int posY, bool filled);

        /*
        ** CapsLockOn
        ** 15px * 15px
        ************/
        static int DrawCapsLockOn(int posX, int posY, bool filled);

        /*
        ** CheckBox
        ** 15px * 15px
        ************/
        static int DrawCheckBox(int posX, int posY, bool isChecked);

        /*
        ** Clear Symbol
        ** 15px * 15px
        *************/
        static int DrawClearSymbol(int posX, int posY, bool filled);

        /*
        ** Clipboard
        ** 25px * 25px
        *************/
        static int DrawClipboard(int posX, int posY, bool filled);

        /*
        ** Close
        ** 25px * 25px
        ************/
        static int DrawClose(int posX, int posY, bool filled);

        /*
        ** Cut
        ** 25px * 25px
        *************/
        static int DrawCut(int posX, int posY, bool filled);

        /*
        ** Duplicate
        ** 25px * 25px
        *************/
        static int DrawDuplicate(int posX, int posY, bool filled);

        /*
        ** Edit
        ** 25px * 25px
        *************/
        static int DrawEdit(int posX, int posY, bool filled);

        /*
        ** Enter Key
        ** 15px * 15px
        ************/
        static int DrawEnterKey(int posX, int posY, bool filled);

        /*
        ** Folder
        ** 15px * 15px
        ************/
        static int DrawFolder(int posX, int posY);

        /*
        ** Favorite
        ** 25px * 25px
        ************/
        static int DrawAddFavorite(int posX, int posY, bool filled);

        /*
        ** File
        ** 15px * 15px
        *************/
        static int DrawFile(int posX, int posY);

        /*
        ** Game Controller
        ** 15px * 15px
        ************/
        static int DrawGameController(int posX, int posY);

        /*
        ** Grid
        ** 15px * 15px
        ************/
        static int DrawGrid(int posX, int posY);

        /*
        ** Info
        ** 25px * 25px
        ***********/
        static int DrawInfo(int posX, int posY, bool filled);

        /*
        ** InfoNew
        ** 25px * 25px
        ***********/
        static int DrawInfoNew(int posX, int posY, bool filled);

        /*
        ** InfoNewDark
        ** 25px * 25px
        ***********/
        static int DrawInfoNewDark(int posX, int posY);

        /*
        ** Discord
        ** 25px * 25px
        **************/
        static int DrawDiscord(int posX, int posY);

        /*
        ** More
        ** 15px * 15px
        **************/
        static int DrawMore(int posX, int posY);

        /*
        ** Plus
        ** 25px * 25px
        **************/
        static int DrawPlus(int posX, int posY, bool filled);

        /*
        ** RAM
        ** 15px * 15 px
        **************/
        static int DrawRAM(int posX, int posY);

        /*
        ** Save Feather
        ** 30px * 30px
        **************/
        static int DrawSave(int posX, int posY);

        /*
        ** Search
        ** 15px * 15 px
        **************/
        static int DrawSearch(int posX, int posY);

        /*
        ** Settings
        ** 15px * 15 px
        **************/
        static int DrawSettings(int posX, int posY);

        /*
        ** Shutdown
        ** 15px * 15 px
        **************/
        static int DrawShutdown(int posX, int posY);

        /*
        ** Tools
        ** 15px * 15 px
        *************/
        static int DrawTools(int posX, int posY);

        /*
        ** Trash
        ** 25px * 25 px
        **************/
        static int DrawTrash(int posX, int posY, bool filled);

        /*
        ** Tricord
        ** 25px * 25 px
        **************/
        static int DrawTricord(int posX, int posY);

        /*
        ** Camera
        ** 15px * 15 px
        **************/
        static int DrawCamera(int posX, int posY);

        /*
         ** LoadPosition
         ** 15px * 15 px
         **************/
        static int DrawLoadPosition(int posX, int posY);

        /*
         ** SavePosition
         ** 15px * 15 px
         **************/
        static int DrawSavePosition(int posX, int posY);

        /*
         ** User Manual
         ** 15px * 15 px
         **************/
        static int DrawManual(int posX, int posY);

        /*
         ** D-Pad and C-Pad, all directions
         ** 15px * 15 px
         **************/
        static int DrawDPad(int posX, int posY, Rotation rotation = ROT_0);
        static int DrawCPad(int posX, int posY, Rotation rotation = ROT_0);

        /*
         ** Zl/ZR
         ** 15px * 15 px
         **************/
        static int DrawZL(int posX, int posY);
        static int DrawZR(int posX, int posY);

        /*
         ** Triforce, filled and unfilled
         ** 25px * 25 px
         **************/
        static int DrawTriforce(int posX, int posY, bool isFilled);

        /*
         ** TFH_Camera
         ** 15px * 15 px
         **************/
        static int DrawTFHCamera(int posX, int posY);

        /*
         ** Link, G/B/R
         ** 30px * 30 px
         **************/
        static int DrawLinkFace(int posX, int posY, int player);


        /*
         ** Facial Expression Editor Icons
         **************/
        static int DrawEye0(int posX, int posY);
        static int DrawEye1(int posX, int posY);
        static int DrawEye2(int posX, int posY);
        static int DrawEye3(int posX, int posY);
        static int DrawEye4(int posX, int posY);
        static int DrawEye5(int posX, int posY);
        static int DrawEye6(int posX, int posY);
        static int DrawEye7(int posX, int posY);
        static int DrawEye8(int posX, int posY);

        static int DrawMayu0(int posX, int posY);
        static int DrawMayu1(int posX, int posY);
        static int DrawMayu2(int posX, int posY);
        static int DrawMayu3(int posX, int posY);
        static int DrawMayu4(int posX, int posY);

        static int DrawMouth0(int posX, int posY);
        static int DrawMouth1(int posX, int posY);
        static int DrawMouth2(int posX, int posY);
        static int DrawMouth3(int posX, int posY);
        static int DrawMouth4(int posX, int posY);
        static int DrawMouth5(int posX, int posY);

        static int DrawFace(int posX, int posY);

        static int DrawLeft(int posX, int posY);
        static int DrawRight(int posX, int posY);

    private:
        static int DrawImg(u8 *img, int posX, int posY, int sizeX, int sizeY);
    };
}

#endif
