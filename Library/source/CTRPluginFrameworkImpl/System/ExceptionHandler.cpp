#include "CTRPluginFrameworkImpl/System/ExceptionHandler.hpp"
#include "CTRPluginFramework/Graphics/Color.hpp"
#include "CTRPluginFramework/Menu/MessageBox.hpp"
#include "CTRPluginFramework/Menu/MenuEntry.hpp"
#include "CTRPluginFramework/Menu/MenuFolder.hpp"
#include "CTRPluginFramework/Menu/PluginMenu.hpp"
#include "CTRPluginFramework/System/Directory.hpp"
#include "CTRPluginFramework/Utils.hpp"

#include "TID.h"
#include "Unicode.h"
#include "csvc.h"
#include "qrcodegen.hpp"
#include <3ds.h>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>

namespace CTRPluginFramework
{
    std::string inviteURL = std::string("discord.gg/") + INVITE;
    std::string exceptionData = "";
    bool savedToSD = false;
    bool showRegisters = false;
    bool showInviteLink = false;

    // Updates exception handler state based on button events
    Process::ExceptionCallbackState ExceptionHandler::getExceptionEventState(void)
    {
        Controller::Update();

        if (Controller::IsKeyPressed(Key::A))
            return Process::EXCB_REBOOT;

        if (Controller::IsKeyPressed(Key::B))
            showRegisters = !showRegisters;

        if (Controller::IsKeyPressed(Key::X) && !savedToSD)
            savedToSD = saveCrashLog();

        if (Controller::IsKeyPressed(Key::Y))
            showInviteLink = !showInviteLink;

        return Process::EXCB_LOOP;
    }

    // Manually draw QR code
    void ExceptionHandler::drawInviteQR(const Screen &screen, u32 posX, u32 posY, qrcodegen::QrCode qrcode)
    {
        int paddingBorder = 25; // width of white border surrounding QR
        int leftBoundary = 15 + paddingBorder;
        int rightBoundary = 165 + paddingBorder;
        int topBoundary = 12 + paddingBorder;
        int bottomBoundary = 165 + paddingBorder;

        // returns QR module count per SIDE...
        int qrSideLen = qrcode.getSize();

        // allocate standardized pixel count per module cell
        int pixelsPerModule = (bottomBoundary - topBoundary) / qrSideLen;

        int currXPos = leftBoundary, currYPos = topBoundary;
        int columnsDrawn = 0, modulesDrawn = 0;

        // draw background...
        screen.DrawRect(5 + paddingBorder, paddingBorder, 167, 167, Color::White);

        // failsafe: it is not possible to draw the QR if module count exceeds available pixel count...
        if (qrSideLen > bottomBoundary - topBoundary)
            return;

        // draw QR modules by columns starting from leftBoundary (columns are drawn top -> bottom)
        while (currXPos < rightBoundary && columnsDrawn < qrSideLen)
        {
            // starting in new column, reset draw status...
            modulesDrawn = 0;
            currYPos = topBoundary;

            // draw modules in current column, starting from top...
            while (currYPos < bottomBoundary && modulesDrawn < qrSideLen)
            {
                screen.DrawRect(currXPos, currYPos, pixelsPerModule, pixelsPerModule, qrcode.getModule(columnsDrawn, modulesDrawn) ? Color::Black : Color::White);
                currYPos += pixelsPerModule;
                modulesDrawn++;
            }
            columnsDrawn++;
            currXPos += pixelsPerModule;
        }
    }

    // Display text info fields
    void ExceptionHandler::drawExceptionInfo(std::string generalInfo, std::string registerInfo)
    {
        int posYOptions = 85;
        int posYIntro = 25;
        int posYCaption = 40;
        int posYLabel = 195;

        const Screen &topScreen = OSD::GetTopScreen();
        const Screen &bottomScreen = OSD::GetBottomScreen();

        // implicit concat works here...
        const unsigned char intro[] = "Oh no! An exception\nhas occurred :(";
        const unsigned char QR_caption[] = "TFH Modding Discord";
        const unsigned char optionA[] = FONT_A ": Reboot";
        const unsigned char optionB[] = FONT_B ": Display register\ndetails below";
        const unsigned char optionBAlt[] = FONT_B ": Hide register\ndetails";
        const unsigned char optionX[] = FONT_X ": Save crash log to\nSD card";
        const unsigned char optionXAlt[] = FONT_X ": Crash log has been\nsaved to SD card";
        const unsigned char optionY[] = FONT_Y ": Show TFH Modding\nDiscord Server invite\nlink";
        const unsigned char optionYAlt[] = FONT_Y ": Display log help\ninfo";
        const unsigned char captionA[] = "Need support assistance? Save the crash\nlog to your SD card and post it as an\nattachment under the #bug-reports\nchannel in the TFH Modding Server.\n\nDon’t forget to describe what happened\nand tag Tricord Team for help! The log\nfile can be found under:\n\nTricord/Crash Logs/[NA/EU/JP]";
        const unsigned char captionB[] = "Need to join the server? The QR invite\nlink is above!\n\nIf you'd prefer to type out the invite link\nyourself, here it is:\n";

        exceptionData = generalInfo + registerInfo + getEnabledEntries();

        // background and QR
        Renderer::SetTarget(TOP);
        topScreen.DrawRect(15, 10, 370, 220, Color::Magenta);
        topScreen.DrawRect(20, 15, 360, 210, Color::Gainsboro, false);
        bottomScreen.DrawRect(15, 20, 290, 205, Color::Magenta);
        bottomScreen.DrawRect(20, 25, 280, 195, Color::Gainsboro, false);

        drawInviteQR(topScreen, 175, 15, qrcodegen::QrCode::encodeText(((std::string("https://discord.com/invite/") + INVITE).c_str()), qrcodegen::QrCode::Ecc::MEDIUM));

        // begin drawing text fields...
        Renderer::DrawGameFontStringReturn(intro, 220, posYIntro, 380, Color::Gainsboro);
        Renderer::DrawGameFontStringReturn(optionA, 220, posYOptions, 380, Color::Gainsboro);

        // button-event controlled text fields...
        if (showRegisters)
        {
            Renderer::DrawGameFontStringReturn(optionBAlt, 220, posYOptions, 380, Color::Gainsboro);
            Renderer::SetTarget(BOTTOM);
            Renderer::DrawGameFontStringReturn((const u8 *)registerInfo.c_str(), 35, posYCaption, 400, Color::Gainsboro);
        }
        else
        {
            Renderer::DrawGameFontStringReturn(optionB, 220, posYOptions, 380, Color::Gainsboro);
            if (!showInviteLink)
            {
                Renderer::SetTarget(BOTTOM);
                Renderer::DrawGameFontStringReturn(captionA, 25, posYCaption, 400, Color::Gainsboro);
            }
            else
            {
                Renderer::SetTarget(BOTTOM);
                Renderer::DrawGameFontStringReturn(captionB, 25, posYCaption, 400, Color::Gainsboro);
                Renderer::DrawGameFontStringReturn((const u8 *)inviteURL.c_str(), 25, posYCaption, 400, Color::Gainsboro);
            }
        }

        Renderer::SetTarget(TOP);

        if (!savedToSD)
            Renderer::DrawGameFontStringReturn(optionX, 220, posYOptions, 380, Color::Gainsboro);
        else
            Renderer::DrawGameFontStringReturn(optionXAlt, 220, posYOptions, 380, Color::Gainsboro);

        if (!showInviteLink)
            Renderer::DrawGameFontStringReturn(optionY, 220, posYOptions, 380, Color::Gainsboro);
        else
            Renderer::DrawGameFontStringReturn(optionYAlt, 220, posYOptions, 380, Color::Gainsboro);

        Renderer::DrawGameFontStringReturn(QR_caption, 50, posYLabel, 380, Color::Gainsboro);

        OSD::SwapBuffers();
    }

    // Retrieves plugin info and exception type as a string
    std::string ExceptionHandler::getErrorInfo(ERRF_ExceptionInfo *excep)
    {
        std::string infoStr = "";

        infoStr += Utils::Format("Tricord version: %s\n", TRICORD_BUILD_METADATA);
        infoStr += Utils::Format("CTRPF version: %s\n", CTRPF_BUILD_METADATA);
        infoStr += Utils::Format("Timestamp: %s%s\n\n", Time::GetDate().c_str(), Time::GetTime().c_str());

        switch (excep->type)
        {
            case ERRF_ExceptionType::ERRF_EXCEPTION_PREFETCH_ABORT:
                infoStr += "Type: Prefetch Abort\n";
                break;
            case ERRF_ExceptionType::ERRF_EXCEPTION_DATA_ABORT:
                infoStr += "Type: Data Abort\n";
                break;
            case ERRF_ExceptionType::ERRF_EXCEPTION_UNDEFINED:
                infoStr += "Type: Undefined Instruction\n";
                break;
            case ERRF_ExceptionType::ERRF_EXCEPTION_VFP:
                infoStr += "Type: VFP Exception\n";
                break;
            default:
                infoStr += "Type: Unknown Exception\n";
                break;
        }
        infoStr += (Utils::Format("Title ID: %016llX", Process::GetTitleID())) + "\n\n";
        return infoStr;
    }

    // Retrieves register info as a string
    std::string ExceptionHandler::getRegisterInfo(ERRF_ExceptionInfo *excep, CpuRegisters *regs)
    {
        std::string registerStr = "";

        // log general registers...
        for (int registerIndex = 0; registerIndex <= 12; registerIndex += 2)
        {
            registerStr += Utils::Format("R%d:   %08X", registerIndex, regs->r[registerIndex]) + "   ";
            if (registerIndex <= 11) // avoid accessing index 12
                registerStr += Utils::Format("R%d:   %08X", registerIndex + 1, regs->r[registerIndex + 1]);
            registerStr += "\n";
        }

        // log special registers...
        registerStr += (Utils::Format("SP:    %08X", regs->sp)) + "   ";
        registerStr += (Utils::Format("CPSR:  %08X", regs->cpsr)) + "\n";
        registerStr += (Utils::Format("LR:    %08X", regs->lr)) + "   ";
        registerStr += (Utils::Format("FPEXC: %08X", excep->fpexc)) + "\n";
        registerStr += (Utils::Format("PC:    %08X", regs->pc)) + "   ";
        registerStr += (Utils::Format("FAR:   %08X", excep->far) + "\n");

        return registerStr;
    }

    // Locates activated entries within subfolders
    void recursiveEntrySearch(std::string &enabledList, MenuFolder *currFolder, std::string parentPath)
    {
        std::vector<MenuEntry *> entries = currFolder->GetEntryList();
        std::vector<MenuFolder *> subfolders = currFolder->GetFolderList();

        // get entries in this current folder...
        for (MenuEntry *entry : entries)
        {
            if (entry->IsActivated())
                enabledList += parentPath + currFolder->Name() + " / " + entry->Name() + "\n";
        }

        // handle subfolders...
        for (MenuFolder *subfolder : subfolders)
        {
            recursiveEntrySearch(enabledList, subfolder, (parentPath + currFolder->Name() + " / "));
        }
    }

    // Retrieves a list of all activated entries
    std::string ExceptionHandler::getEnabledEntries(void)
    {
        std::string enabledList = "\nActive Entries:\n\n";
        PluginMenu *menu = PluginMenu::GetRunningInstance();

        if (menu == nullptr)
            return "/nNULL: Menu did not initialize successfully.";

        // get any freestanding entries...
        std::vector<MenuEntry *> entryList = menu->GetEntryList();
        for (MenuEntry *entry : entryList)
        {
            if (entry->IsActivated())
                enabledList += entry->Name() + "\n";
        }

        // get entries stored in folders via recursive search to handle nested subfolders...
        std::vector<MenuFolder *> folderList = menu->GetFolderList();
        for (MenuFolder *folder : folderList)
            recursiveEntrySearch(enabledList, folder);

        std::vector<MenuFolder *> hiddenFolders = menu->GetHiddenList();
        for (MenuFolder *folder : hiddenFolders)
            recursiveEntrySearch(enabledList, folder);

        return enabledList;
    }

    // Writes the crash log to the SD card
    bool ExceptionHandler::saveCrashLog(void)
    {
        std::string logPath = "/Tricord/Crash Logs/";

        // long directories can't be made in one go -> split into two create events
        if (!Directory::IsExists(logPath))
            Directory::Create(logPath);

        // build path to log file...
        logPath.append(Process::GetRegionCode() + "/");

        if (!Directory::IsExists(logPath))
            Directory::Create(logPath);

        logPath.append(Utils::Format("Crash Log - %s%s.txt", Time::GetDate().c_str(), Time::GetTime().c_str()));

        if (!File::Exists(logPath))
            File::Create(logPath);

        // write log contents...
        File logFile(logPath, File::WRITE | File::TRUNCATE);
        if (!logFile.IsOpen() || !File::Exists(logPath))
        {
            MessageBox("Error", "Crash log could not be saved to SD card.", DialogType::DialogOk)();
            return false;
        }

        logFile.Write(exceptionData.c_str(), strlen(exceptionData.c_str()));
        logFile.Flush();
        logFile.Close();

        return true;
    }
}