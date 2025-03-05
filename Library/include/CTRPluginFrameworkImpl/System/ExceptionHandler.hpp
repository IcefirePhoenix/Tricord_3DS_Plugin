#ifndef CTRPLUGINFRAMEWORKIMPL_EXCEPTION_HANDLER_HPP
#define CTRPLUGINFRAMEWORKIMPL_EXCEPTION_HANDLER_HPP

#include "CTRPluginFrameworkImpl/Graphics.hpp"
#include "CTRPluginFrameworkImpl/Menu/MenuFolderImpl.hpp"
#include "CTRPluginFrameworkImpl/System.hpp"
#include "CTRPluginFrameworkImpl/System/Screen.hpp"
#include "qrcodegen.hpp"

namespace CTRPluginFramework
{
    void recursiveEntrySearch(std::string &enabledList, MenuFolder *currFolder, std::string parentPath = "");
    class ExceptionHandler
    {
    public:
        static Process::ExceptionCallbackState getExceptionEventState(void);
        static void drawInviteQR(const Screen &screen, u32 posX, u32 posY, qrcodegen::QrCode qrcode);
        static void drawExceptionInfo(std::string generalInfo, std::string registerInfo);
        static std::string getErrorInfo(ERRF_ExceptionInfo *excep);
        static std::string getRegisterInfo(ERRF_ExceptionInfo *excep, CpuRegisters *regs);
        static std::string getEnabledEntries(void);
        static bool saveCrashLog(void);
    };
}

#endif
