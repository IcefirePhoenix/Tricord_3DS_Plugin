#ifndef FACEEXPREDITOR_HPP
#define FACEEXPREDITOR_HPP

#include "Helpers.hpp"

namespace CTRPluginFramework
{
    using StringVector = std::vector<std::string>;
    using IconCallback = int (*)(int, int);

    struct IconLabel
    {
        float floatVal;
        IconCallback icon;
        std::string label;
    };

    class FaceExprEditor
    {
    public:
        FaceExprEditor(int frameIndex, std::string &frameLabel);
        ~FaceExprEditor(void);
        void operator()(void);

        static bool expandMasterTexRefBlock(u32 LFC_MA_masterStartAddr);
        static bool restoreEyeTexMasterList(u32 LFC_MA_masterStartAddr);
        static bool initCustomCH(u32 LFC_MA_masterStartAddr);
        static bool copyEyeAnimData(u32 LFC_MA_masterStartAddr);
        static bool copyMayuAnimData(u32 LFC_MA_masterStartAddr);
        static bool copyMouthAnimData(u32 LFC_MA_masterStartAddr);
        static void writeCategoryAnimMetadata(u32 writerStartAddr, int curveIndex);
        static bool buildTexRefPtrBlock(void);

        static void editChild_FC_MA_Blocks(void);
        static bool copyRedirIH_PtrBlock(u32 LFC_MA_startAddr);
        static bool redirCH(u32 LFC_MA_startAddr);
        static bool redirTexRefPtr(u32 LFC_MA_startAddr);
        static void editMngr(MenuEntry *entry);

        void updateAnimData(void);

        std::string _frameLabel;
        int _frameIndex;
    };
}

#endif