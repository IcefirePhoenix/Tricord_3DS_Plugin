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

        static StringVector frameList;

        static void initSeq(void);
        static bool buildCustomData(void);
        static bool loadSavedSelectionsFromFile(void);
        static bool restoreEyeTexMasterList(u32 LFC_MA_masterStartAddr);
        static bool initCustomCH(u32 LFC_MA_masterStartAddr);
        static bool copyEyeAnimData(u32 LFC_MA_masterStartAddr);
        static bool copyMayuAnimData(u32 LFC_MA_masterStartAddr);
        static bool copyMouthAnimData(u32 LFC_MA_masterStartAddr);
        static void writeCategoryAnimMetadata(u32 writerStartAddr, int curveIndex);
        static bool buildTexRefPtrBlock(void);
        static void updateLabels(bool updateAll, int workingIndex = 0);
        static void editChild_FC_MA_Blocks(void);
        static bool copyRedirIH_PtrBlock(u32 LFC_MA_startAddr);
        static bool redirCH(u32 LFC_MA_startAddr);
        static bool redirTexRefPtr(u32 LFC_MA_startAddr);
        static bool alterTexBlockSize(u32 LFC_MA_startAddr, bool restore);
        static void resetExprs(void);

        static void editMngr(MenuEntry *entry);

        void updateAnimData(void);

    private:
        void _drawTop(void);
        void _drawBottom(void);
        bool _updateIcon(int faceSection, bool goingLeft);
        void _updateMenuGraphics(void);

        std::vector<Button> _leftArrs;
        std::vector<Button> _rightArrs;
        std::string _frameLabel;
        int _frameIndex;
    };
}

#endif