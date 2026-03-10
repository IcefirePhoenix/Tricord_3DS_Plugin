#include "types.h"
#include "3ds.h"

#include "CTRPluginFramework/Graphics.hpp"
#include "CTRPluginFrameworkImpl/Graphics.hpp"

namespace CTRPluginFramework
{
    extern "C" unsigned char *About15;
    extern "C" unsigned char *BlueLink30;
    extern "C" unsigned char *HeartEmpty25;
    extern "C" unsigned char *HeartFilled25;
    extern "C" unsigned char *ButtonActive;
    extern "C" unsigned char *ButtonInactive;
    extern "C" unsigned char *BottomBG_Underline;
    extern "C" unsigned char *CheckedCheckbox;
    extern "C" unsigned char *UnCheckedCheckbox;
    extern "C" unsigned char *Camera15;
    extern "C" unsigned char *Console25;
    extern "C" unsigned char *ConsoleDisabled25;
    extern "C" unsigned char *CapsLockOn15;
    extern "C" unsigned char *CapsLockOnFilled15;
    extern "C" unsigned char *ClearSymbol15;
    extern "C" unsigned char *ClearSymbolFilled15;
    extern "C" unsigned char *Clipboard25;
    extern "C" unsigned char *ClipboardFilled25;
    extern "C" unsigned char *CloseWindow20;
    extern "C" unsigned char *CloseWindowFilled20;
    extern "C" unsigned char *Controller15;
    extern "C" unsigned char *Cut25;
    extern "C" unsigned char *CutFilled25;
    extern "C" unsigned char *Discord25;
    extern "C" unsigned char *CPad15;
    extern "C" unsigned char *DPad15;
    extern "C" unsigned char *Duplicate25;
    extern "C" unsigned char *DuplicateFilled25;
    extern "C" unsigned char *Edit25;
    extern "C" unsigned char *EditFilled25;
    extern "C" unsigned char *EnterKey15;
    extern "C" unsigned char *EnterKeyFilled15;
    extern "C" unsigned char *FolderFilled;
    extern "C" unsigned char *File15;
    extern "C" unsigned char *GameController15;
    extern "C" unsigned char *GreenLink30;
    extern "C" unsigned char *Grid15;
    extern "C" unsigned char *Info25;
    extern "C" unsigned char *InfoFilled25;
    extern "C" unsigned char *InfoNewSelect25;
    extern "C" unsigned char *InfoNew25;
    extern "C" unsigned char *InfoNew25Dark;
    extern "C" unsigned char *LoadPosition15;
    extern "C" unsigned char *SavePosition15;
    extern "C" unsigned char *Maintenance15;
    extern "C" unsigned char *More15;
    extern "C" unsigned char *Plus25;
    extern "C" unsigned char *PlusFilled25;
    extern "C" unsigned char *RAM15;
    extern "C" unsigned char *RedLink30;
    extern "C" unsigned char *ReturnButton25;
    extern "C" unsigned char *ReturnButtonPush25;
    extern "C" unsigned char *SavePink30;
    extern "C" unsigned char *Search15;
    extern "C" unsigned char *Settings15;
    extern "C" unsigned char *Shutdown15;
    extern "C" unsigned char *TFH_Camera;
    extern "C" unsigned char *Trash25;
    extern "C" unsigned char *TrashFilled25;
    extern "C" unsigned char *TricordIcon25;
    extern "C" unsigned char *Triforce25;
    extern "C" unsigned char *TriforceFilled25;
    extern "C" unsigned char *ZL15;
    extern "C" unsigned char *ZR15;
    extern "C" unsigned char *UserManual15;

    extern "C" unsigned char *Eye0;
    extern "C" unsigned char *Eye1;
    extern "C" unsigned char *Eye2;
    extern "C" unsigned char *Eye3;
    extern "C" unsigned char *Eye4;
    extern "C" unsigned char *Eye5;
    extern "C" unsigned char *Eye6;
    extern "C" unsigned char *Eye7;
    extern "C" unsigned char *Eye8;

    extern "C" unsigned char *Mayu0;
    extern "C" unsigned char *Mayu1;
    extern "C" unsigned char *Mayu2;
    extern "C" unsigned char *Mayu3;
    extern "C" unsigned char *Mayu4;

    extern "C" unsigned char *Mouth0;
    extern "C" unsigned char *Mouth1;
    extern "C" unsigned char *Mouth2;
    extern "C" unsigned char *Mouth3;
    extern "C" unsigned char *Mouth4;
    extern "C" unsigned char *Mouth5;

    extern "C" unsigned char *MainFace;

    extern "C" unsigned char *LeftArr;
    extern "C" unsigned char *RightArr;

    #define RGBA8 GSP_RGBA8_OES

    struct Pixel
    {
        u8 a;
        u8 b;
        u8 g;
        u8 r;
    };


    std::vector<u8> RotateIcon(const u8* src, int width, int height, Icon::Rotation rotation)
    {
        int bpp = 4; // bytes-per-pixel, 4 = RGBA
        int new_width  = (rotation == Icon::ROT_90 || rotation == Icon::ROT_270) ? height : width;
        int new_height = (rotation == Icon::ROT_90 || rotation == Icon::ROT_270) ? width : height;

        std::vector<u8> rotated(new_width * new_height * bpp, 0);

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int src_index = (y * width + x) * bpp;  // pixel index in source image
                int dst_index = 0;                      // pixel index in dest image

                switch (rotation)
                {
                    case Icon::ROT_0:
                        dst_index = src_index;
                        break;
                    case Icon::ROT_90:
                        dst_index = (x * new_height + (new_height - 1 - y)) * bpp;
                        break;
                    case Icon::ROT_180:
                        dst_index = ((new_height - 1 - y) * new_width + (new_width - 1 - x)) * bpp;
                        break;
                    case Icon::ROT_270:
                        dst_index = ((new_width - 1 - x) * new_height + y) * bpp;
                        break;
                }

                // RGBA, 4 color channels
                for (int colorChannel = 0; colorChannel < bpp; colorChannel++)
                {
                    rotated[dst_index + colorChannel] = src[src_index + colorChannel];
                }
            }
        }

        return rotated;
    }


    inline int Icon::DrawImg(u8 *img, int posX, int posY, int sizeX, int sizeY)
    {
        u8      *framebuf = nullptr;
        u8      *imgb = img;
        u32     target = Renderer::GetContext()->target;
        int     rowstride;
        int     bpp;
        bool    is3d = false;
        Color   px;

        posY += sizeY;
        GSPGPU_FramebufferFormat fmt;
        // Get target infos
        switch (target)
        {
            case Target::TOP:
            {
                framebuf = ScreenImpl::Top->GetLeftFrameBuffer(posX, posY);
                ScreenImpl::Top->GetFrameBufferInfos(rowstride, bpp, fmt);
                break;
            }
            case Target::BOTTOM:
            {
                framebuf = ScreenImpl::Bottom->GetLeftFrameBuffer(posX, posY);
                ScreenImpl::Bottom->GetFrameBufferInfos(rowstride, bpp, fmt);
                break;
            }
            default:
                return (posX);
        }
    again3D:
        if (framebuf == nullptr)
            return (posX);

        // Draw
        for (int x = 0; x < sizeX; x++)
        {
            u8 *dst = framebuf + rowstride * x;
            int y = 0;
            while (y++ < sizeY)
            {
                Pixel *pix = (Pixel *)img;
                px.a = pix->a;
                px.r = pix->r;
                px.g = pix->g;
                px.b = pix->b;

                // Skip pixels with less than 10% visibility
                if (px.a > 25)
                {
                    Color &&bg = PrivColor::FromFramebuffer(dst);
                    Color &&blended = bg.Blend(px, Color::BlendMode::Alpha);
                    dst = PrivColor::ToFramebuffer(dst, blended);
                }
                else
                    dst += bpp; // advance by bytes-per-pixel (4 = RGBA)
                img += 4;
            }
        }

        if (!is3d && target == Target::TOP && ScreenImpl::Top->Is3DEnabled())
        {
            framebuf = ScreenImpl::Top->GetRightFrameBuffer(posX, posY);
            img = imgb;
            is3d = true;
            goto again3D;
        }

        return (posX + sizeX);
    }

    /*
    ** 3DS
    ** 25px * 25px
    **************/
    int Icon::Draw3DS(int posX, int posY, bool enabled)
    {
        u8 *img = enabled ? Console25 : ConsoleDisabled25;

        return (DrawImg(img, posX, posY, 25, 25));
    }

    /*
    ** About
    ** 15px * 15px
    **************/
    int Icon::DrawAbout(int posX, int posY)
    {
        return (DrawImg(About15, posX, posY, 15, 15));
    }

    /*
    ** BG_Underline
    ** 304px * 22px
    **************/
    int Icon::DrawBG_Underline(int posX, int posY)
    {
        return (DrawImg(BottomBG_Underline, posX, posY, 304, 22));
    }

    /*
    ** CapsLockOn
    ** 15px * 15px
    ************/
    int Icon::DrawCapsLockOn(int posX, int posY, bool isFilled)
    {
        u8 *img = isFilled ? CapsLockOnFilled15 : CapsLockOn15;
        return (DrawImg(img, posX, posY, 15, 15));
    }

    /*
    ** CheckBox
    ** 15px * 15px
    ************/
    int Icon::DrawCheckBox(int posX, int posY, bool isChecked)
    {
        u8 *img = isChecked ? CheckedCheckbox : UnCheckedCheckbox;
        return (DrawImg(img, posX, posY, 15, 15));
    }

    /*
    ** ClearSymbol
    ** 15px * 15px
    **********/
    int Icon::DrawClearSymbol(int posX, int posY, bool filled)
    {
        u8 *img = filled ? ClearSymbolFilled15 : ClearSymbol15;

        return (DrawImg(img, posX, posY, 15, 15));
    }

    /*
    ** Clipboard
    ** 25px * 25px
    *************/
    int Icon::DrawClipboard(int posX, int posY, bool filled)
    {
        u8 *img = filled ? ClipboardFilled25 : Clipboard25;
        return (DrawImg(img, posX, posY, 25, 25));
    }

    /*
    ** Close
    ** 25px * 25px
    **********/
    int Icon::DrawClose(int posX, int posY, bool filled)
    {
        u8 *img = filled ? ReturnButtonPush25 : ReturnButton25;
        return (DrawImg(img, posX, posY, 25, 25));
    }

    /*
     ** MenuButton
     ** 120px * 32px
     **********/
    int Icon::DrawMenuButton(int posX, int posY, bool filled)
    {
        u8 *img = filled ? ButtonActive : ButtonInactive;
        return (DrawImg(img, posX, posY, 120, 32));
    }

    /*
    ** Cut
    ** 25px * 25px
    *************/
    int Icon::DrawCut(int posX, int posY, bool filled)
    {
        u8 *img = filled ? CutFilled25 : Cut25;
        return (DrawImg(img, posX, posY, 25, 25));
    }

    /*
    ** Cut
    ** 25px * 25px
    *************/
    int Icon::DrawDuplicate(int posX, int posY, bool filled)
    {
        u8 *img = filled ? DuplicateFilled25 : Duplicate25;
        return (DrawImg(img, posX, posY, 25, 25));
    }

    /*
    ** Edit
    ** 25px * 25px
    *************/
    int Icon::DrawEdit(int posX, int posY, bool filled)
    {
        u8 *img = filled ? EditFilled25 : Edit25;
        return (DrawImg(img, posX, posY, 25, 25));
    }

    /*
    ** Enter Key
    ** 15px * 15px
    **************/
    int Icon::DrawEnterKey(int posX, int posY, bool filled)
    {
        u8 *img = filled ? EnterKeyFilled15 : EnterKey15;
        return (DrawImg(img, posX, posY, 15, 15));
    }

    /*
    ** Folder
    ** 15px * 15px
    ***************/
    int Icon::DrawFolder(int posX, int posY)
    {
        return (DrawImg(FolderFilled, posX, posY, 15, 15));
    }

    /*
    ** Favorite
    ** 25px * 25px
    ***************/
    int Icon::DrawAddFavorite(int posX, int posY, bool filled)
    {
        u8 *img = filled ? HeartFilled25 : HeartEmpty25;
        return (DrawImg(img, posX, posY, 25, 25));
    }

    /*
    ** File
    ** 15px * 15px
    **************/
    int Icon::DrawFile(int posX, int posY)
    {
        return (DrawImg(File15, posX, posY, 15, 15));
    }

    /*
    ** Game Controller
    ** 15px * 15px
    **************/
    int Icon::DrawGameController(int posX, int posY)
    {
        return (DrawImg(GameController15, posX, posY, 15, 15));
    }

    /*
    ** Grid
    ** 15px * 15px
    **************/
    int Icon::DrawGrid(int posX, int posY)
    {
        return (DrawImg(Grid15, posX, posY, 15, 15));
    }

    /*
    ** Info
    ** 25px * 25px
    ***************/
    int Icon::DrawInfo(int posX, int posY, bool filled)
    {
        u8 *img = filled ? InfoFilled25 : Info25;
        return (DrawImg(img, posX, posY, 25, 25));
    }

    /*
    ** InfoNew
    ** 25px * 25px
    ***************/
    int Icon::DrawInfoNew(int posX, int posY, bool filled)
    {
        u8 *img = filled ? InfoNewSelect25 : InfoNew25;
        return (DrawImg(img, posX, posY, 25, 25));
    }

    /*
    ** InfoNewDark
    ** 25px * 25px
    ***************/
    int Icon::DrawInfoNewDark(int posX, int posY)
    {
        return (DrawImg(InfoNew25Dark, posX, posY, 25, 25));
    }

    /*
    ** Discord
    ** 25px * 25px
    **************/
    int Icon::DrawDiscord(int posX, int posY)
    {
        return (DrawImg(Discord25, posX, posY, 25, 25));
    }

    /*
    ** More
    ** 15px * 15px
    **************/
    int Icon::DrawMore(int posX, int posY)
    {
        return (DrawImg(More15, posX, posY, 15, 15));
    }

    /*
    ** Plus
    ** 25px * 25px
    **************/
    int Icon::DrawPlus(int posX, int posY, bool filled)
    {
        u8 *img = filled ? PlusFilled25 : Plus25;
        return (DrawImg(img, posX, posY, 25, 25));
    }

    /*
    ** RAM
    ** 15px * 15px
    ***************/

    int Icon::DrawRAM(int posX, int posY)
    {
        return (DrawImg(RAM15, posX, posY, 15, 15));
    }

    /*
    ** Save Feather
    ** 30px * 30px
    **************/
    int Icon::DrawSave(int posX, int posY)
    {
        return (DrawImg(SavePink30, posX, posY, 30, 30));
    }

    /*
    ** Search
    ** 15px * 15px
    ***************/

    int Icon::DrawSearch(int posX, int posY)
    {
        return (DrawImg(Search15, posX, posY, 15, 15));
    }

    /*
    ** Settings
    ** 15px * 15px
    ***************/

    int Icon::DrawSettings(int posX, int posY)
    {
        return (DrawImg(Settings15, posX, posY, 15, 15));
    }

    /*
    ** Shutdown
    ** 15px * 15 px
    **************/
    int Icon::DrawShutdown(int posX, int posY)
    {
        return (DrawImg(Shutdown15, posX, posY, 15, 15));
    }

    /*
    ** Tools
    ** 15px * 15px
    ***************/

    int Icon::DrawTools(int posX, int posY)
    {
        return (DrawImg(Maintenance15, posX, posY, 15, 15));
    }

    /*
    ** Trash
    ** 25px * 25px
    ***************/
    int Icon::DrawTrash(int posX, int posY, bool filled)
    {
        u8 *img = filled ? TrashFilled25 : Trash25;
        return (DrawImg(img, posX, posY, 25, 25));
    }

    /*
    ** Tricord
    ** 25px * 25px
    ***************/
    int Icon::DrawTricord(int posX, int posY)
    {
        return (DrawImg(TricordIcon25, posX, posY, 25, 25));
    }

    /*
    ** Camera
    ** 15px * 15 px
    **************/
    int Icon::DrawCamera(int posX, int posY)
    {
        return (DrawImg(Camera15, posX, posY, 15, 15));
    }

    /*
     ** LoadPosition
     ** 15px * 15 px
     **************/
    int Icon::DrawLoadPosition(int posX, int posY)
    {
        return (DrawImg(LoadPosition15, posX, posY, 15, 15));
    }

    /*
     ** SavePosition
     ** 15px * 15 px
     **************/
    int Icon::DrawSavePosition(int posX, int posY)
    {
        return (DrawImg(SavePosition15, posX, posY, 15, 15));
    }

    /*
     ** User Manual
     ** 15px * 15 px
     **************/
    int Icon::DrawManual(int posX, int posY)
    {
        return (DrawImg(UserManual15, posX, posY, 15, 15));
    }

    /*
     ** ZL/ZR
     ** 15px * 15 px
     **************/
    int Icon::DrawZL(int posX, int posY)
    {
        return (DrawImg(ZL15, posX, posY, 15, 15));
    }

    int Icon::DrawZR(int posX, int posY)
    {
        return (DrawImg(ZR15, posX, posY, 15, 15));
    }

    /*
     ** Triforce (unfilled and filled)
     ** 25px * 25 px
     **************/
    int Icon::DrawTriforce(int posX, int posY, bool isFilled)
    {
        u8 *img = isFilled ? TriforceFilled25 : Triforce25;
        return (DrawImg(img, posX, posY, 25, 25));
    }

    /*
     ** TFH Camera
     ** 15px * 15 px
     **************/
    int Icon::DrawTFHCamera(int posX, int posY)
    {
        return (DrawImg(TFH_Camera, posX, posY, 20, 20));
    }

    /*
     ** D-Pad, all directions
     ** 15px * 15 px
     **************/
    int Icon::DrawDPad(int posX, int posY, Rotation rotation)
    {
        u8* rotated = rotation == ROT_0 ? DPad15 : RotateIcon(DPad15, 15, 15, rotation).data();

        return (DrawImg(rotated, posX, posY, 15, 15));
    }

    /*
     ** C-Pad, all directions
     ** 15px * 15 px
     **************/
    int Icon::DrawCPad(int posX, int posY, Rotation rotation)
    {
        u8* rotated = rotation == ROT_0 ? CPad15 : RotateIcon(CPad15, 15, 15, rotation).data();

        return (DrawImg(rotated, posX, posY, 15, 15));
    }

    /*
    ** Link, G/B/R
    ** 30px * 30 px
    **************/
    int Icon::DrawLinkFace(int posX, int posY, int player)
    {
        u8 *link = GreenLink30;
        switch (player)
        {
            case 1:
                link = BlueLink30;
                break;
            case 2:
                link = RedLink30;
                break;
            default:
                break;
        }

        return (DrawImg(link, posX, posY, 30, 30));
    }

    /* Facial Expression Editor */
    int Icon::DrawEye0(int posX, int posY)
    {
        return (DrawImg(Eye0, posX, posY, 40, 15));
    }

    int Icon::DrawEye1(int posX, int posY)
    {
        return (DrawImg(Eye1, posX, posY, 40, 15));
    }

    int Icon::DrawEye2(int posX, int posY)
    {
        return (DrawImg(Eye2, posX, posY, 40, 15));
    }

    int Icon::DrawEye3(int posX, int posY)
    {
        return (DrawImg(Eye3, posX, posY, 40, 15));
    }

    int Icon::DrawEye4(int posX, int posY)
    {
        return (DrawImg(Eye4, posX, posY, 40, 15));
    }

    int Icon::DrawEye5(int posX, int posY)
    {
        return (DrawImg(Eye5, posX, posY, 40, 15));
    }

    int Icon::DrawEye6(int posX, int posY)
    {
        return (DrawImg(Eye6, posX, posY, 40, 15));
    }

    int Icon::DrawEye7(int posX, int posY)
    {
        return (DrawImg(Eye7, posX, posY, 40, 15));
    }

    int Icon::DrawEye8(int posX, int posY)
    {
        return (DrawImg(Eye8, posX, posY, 40, 15));
    }

    int Icon::DrawMayu0(int posX, int posY)
    {
        return (DrawImg(Mayu0, posX, posY, 40, 15));
    }

    int Icon::DrawMayu1(int posX, int posY)
    {
        return (DrawImg(Mayu1, posX, posY, 40, 15));
    }

    int Icon::DrawMayu2(int posX, int posY)
    {
        return (DrawImg(Mayu2, posX, posY, 40, 15));
    }

    int Icon::DrawMayu3(int posX, int posY)
    {
        return (DrawImg(Mayu3, posX, posY, 40, 15));
    }

    int Icon::DrawMayu4(int posX, int posY)
    {
        return (DrawImg(Mayu4, posX, posY, 40, 15));
    }

    int Icon::DrawMouth0(int posX, int posY)
    {
        return (DrawImg(Mouth0, posX, posY, 20, 8));
    }

    int Icon::DrawMouth1(int posX, int posY)
    {
        return (DrawImg(Mouth1, posX, posY, 20, 8));
    }

    int Icon::DrawMouth2(int posX, int posY)
    {
        return (DrawImg(Mouth2, posX, posY, 20, 8));
    }

    int Icon::DrawMouth3(int posX, int posY)
    {
        return (DrawImg(Mouth3, posX, posY, 20, 8));
    }

    int Icon::DrawMouth4(int posX, int posY)
    {
        return (DrawImg(Mouth4, posX, posY, 20, 8));
    }

    int Icon::DrawMouth5(int posX, int posY)
    {
        return (DrawImg(Mouth5, posX, posY, 20, 8));
    }

    int Icon::DrawLeft(int posX, int posY)
    {
        return (DrawImg(LeftArr, posX, posY, 25, 25));
    }

    int Icon::DrawRight(int posX, int posY)
    {
        return (DrawImg(RightArr, posX, posY, 25, 25));
    }

    int Icon::DrawFace(int posX, int posY)
    {
        return (DrawImg(MainFace, posX, posY, 75, 90));
    }
}
