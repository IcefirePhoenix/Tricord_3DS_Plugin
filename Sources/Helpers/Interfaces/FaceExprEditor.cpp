#include "Helpers.hpp"

namespace CTRPluginFramework
{
    constexpr int frameCount = 12,
        originalTexCount = 16,
        newTexCount = 20;
    constexpr u8 increasedTexListSize = 0x14,
        animMetadataSize = 0x24;
    constexpr u16 eyeLabelOffset = 0xE95,
        mayuLabelOffset = 0xF15,
        mouthLabelOffset = 0xF44;

    const std::vector<IconLabel> eyeInfo =
    {
        {0.0, Icon::DrawEye0, "Idle"},
        {16.0, Icon::DrawEye1, "Looking left"},
        {17.0, Icon::DrawEye2, "Looking right"},
        {1.0, Icon::DrawEye3, "Happily shut"},
        {2.0, Icon::DrawEye4, "Squinting"},
        {18.0, Icon::DrawEye5, "Peacefully shut"},
        {3.0, Icon::DrawEye6, "Hurt"},
        {4.0, Icon::DrawEye7, "Half-open"},
        {5.0, Icon::DrawEye8, "Shocked"}
    };

    const std::vector<IconLabel> mayuInfo =
    {
        {6.0, Icon::DrawMayu0, "Idle"},
        {7.0, Icon::DrawMayu1, "Scrunched"},
        {8.0, Icon::DrawMayu2, "Curved upward"},
        {9.0, Icon::DrawMayu3, "Inquisitive"},
        {10.0, Icon::DrawMayu4, "Pitiful"}
    };

    const std::vector<IconLabel> mouthInfo =
    {
        {11.0, Icon::DrawMouth0, "Idle"},
        {12.0, Icon::DrawMouth1, "Wide open"},
        {13.0, Icon::DrawMouth2, "Teeth clenched"},
        {14.0, Icon::DrawMouth3, "Partly open"},
        {15.0, Icon::DrawMouth4, "Smiling"},
        {19.0, Icon::DrawMouth5, "Sad"}
    };

    StringVector currLabels =
    {
        eyeInfo[0].label,
        mayuInfo[0].label,
        mouthInfo[0].label
    };

    int currExprIndexes[3] = {0, 0, 0};
    int dataSizes[3] = {9, 5, 6};

    FaceExprEditor::FaceExprEditor(int frameIndex, std::string &frameLabel) : _frameIndex(frameIndex), _frameLabel(frameLabel)
    {
        // _getExprIndexes(exprIndex);

        for (int column1 = 0, posY = 90; column1 < 3; column1++, posY += 44)
        {
            Button newLButton(Button::Icon, IntRect(100, posY, 25, 25), Icon::DrawLeft);
            Button newRButton(Button::Icon, IntRect(260, posY, 25, 25), Icon::DrawRight);

            _leftArrs.push_back(newLButton);
            _rightArrs.push_back(newRButton);
        }
    }

    FaceExprEditor::~FaceExprEditor()
    {
    }

    void FaceExprEditor::operator()(void)
    {
        bool mustclose = false;
        bool sleepClose = false;

        while (((!Window::BottomWindow.MustClose() && !mustclose)) && !sleepClose)
        {
            Controller::Update();
            mustclose = Controller::IsKeyPressed(Key::B);
            sleepClose = SystemImpl::IsSleeping();

            _drawTop();
            _drawBottom();

            Renderer::EndFrame();
            _updateMenuGraphics();
        }

        updateAnimData();
        // call Settings::SaveExprOpts()
    }

    /* -------------- EXECUTED ONCE DURING INIT -------------- */

    // Expands the Master LFC_MA texture filename reference pointer block
    // Note: any edits made to the Master LFC_MA block automatically propagate to the LFC_MA child blocks
    bool FaceExprEditor::expandMasterTexRefBlock(u32 LFC_MA_masterStartAddr)
    {
        u8 masterListSizeOffset = 0x4;

        // update list size
        if (!Process::Write8(LFC_MA_masterStartAddr + masterListSizeOffset, increasedTexListSize))
        {
            OSD::Notify("[ERROR] Editing LFC_MA master filename list size failed.", Color::Red);
            return false;
        }
        return true;
    }

    // Restores reference to unused texture "eye.1" to the Master Link_anm_mat.bch texture filename list using an extra, unused string
    // Note: this edit propagates to all child LFC_MA blocks
    bool FaceExprEditor::restoreEyeTexMasterList(u32 LFC_MA_masterStartAddr)
    {
        // Link_anm_mat.bch does not contain any animations using the texture "eye.1" hence this string needs to be placed in memory... simply replace extra/unused string "eye.9" with "eye.1"
        std::string customStr = "eye.1";
        u16 eye9_TexOffset = 0xEC9;

        if (!Process::WriteString(LFC_MA_masterStartAddr + eye9_TexOffset, customStr, StringFormat::Utf8))
        {
            OSD::Notify("[ERROR] Restoring eye.1 texture filename failed.", Color::Red);
            return false;
        }
        return true;
    }

    // Recreates Category Header data; will be used universally across all LFC_MA child blocks
    bool FaceExprEditor::initCustomCH(u32 LFC_MA_masterStartAddr)
    {
        u8 CH_entrySize = 0x10;
        u32 CH_start = AddressList::getAddress("CustomCategoryHeaders");
        u32 writerStartAddr = CH_start;
        u32 labelAddrs[3] =
        {
            LFC_MA_masterStartAddr + eyeLabelOffset,
            LFC_MA_masterStartAddr + mayuLabelOffset,
            LFC_MA_masterStartAddr + mouthLabelOffset
        };

        // CH struct:
        // u32 pointer to category label string (set to LFC_MA master labels)
        // u16 TargetType (always 0x000D)
        // u16 PrimitiveType (always 0x0006)
        // u16 unknown (always 0x0000)
        // u16 unknownID (always 0x0E00)
        // u32 pointer to category's animation metadata (set to secondary/custom animation blocks)

        u16 targetType = 0x000D, primitiveType = 0x0006;
        u32 unknown = 0x00000E00;

        for (int category = 0; category < 3; category++)
        {
            Process::Write32(writerStartAddr, labelAddrs[category]);
            Process::Write16(writerStartAddr + sizeof(u32), targetType);
            Process::Write16(writerStartAddr + sizeof(u32) + sizeof(u16), primitiveType);
            Process::Write32(writerStartAddr + sizeof(u64), unknown);
            Process::Write32(writerStartAddr + sizeof(u64) + sizeof(u32), AddressList::getAddress("CustomAnimData") + (sizeof(u64) * frameCount * category) + (animMetadataSize * category));

            writerStartAddr += CH_entrySize;
        }

        // set up CH pointers
        u32 CH_ptrs = AddressList::getAddress("RedirCategoryHeaderPtrs");
        Process::Write32(CH_ptrs, CH_start);
        Process::Write32(CH_ptrs + sizeof(u32), CH_start + CH_entrySize);
        Process::Write32(CH_ptrs + sizeof(u32) * 2, CH_start + CH_entrySize * 2);

        return true;
    }

    // Copies LFC_MA eye, eyebrow (mayu), and mouth data to padding; this will serve as a secondary copy where custom animation edits are applied, allowing the original data to remain untouched for easy reverts by simply cancelling redirects
    bool FaceExprEditor::copyEyeAnimData(u32 LFC_MA_masterStartAddr)
    {
        /**
         * Eye data needs to be converted to StepLinear64 instead of StepLinear32
         * StepLinear32 parsing reference: https://github.com/gdkchan/SPICA/blob/master/SPICA/Formats/Common/KeyFrameQuantizationHelper.cs
         *
         * Algorithm to convert between texture ID to float:
         *
         * 1) obtain upper 20 bits of texture ID -> this is frameVal
         * 2) mask frameVal to isolate single digit (note that all digits are the same)
         * 3) divide isolated digit by max single digit val (4-bit limit, 0xF)
         * 4) convert result into float
         * 5) multiply by max eye texture index (5, see data spreadsheet)
         */

        float eyeFrameValBuffer;
        u8 eyeOffset = 0xA8;
        u32 writerCurrAddress = AddressList::getAddress("CustomAnimData");
        u32 currentEyeAnimAddress = LFC_MA_masterStartAddr + eyeOffset;
        u32 tmpFrameDataBuffer;

        writeCategoryAnimMetadata(writerCurrAddress, 0);
        writerCurrAddress += animMetadataSize;

        for (int iter = 0; iter < frameCount; iter++)
        {
            Process::Read32(currentEyeAnimAddress, tmpFrameDataBuffer);

            int frameNum = (tmpFrameDataBuffer >> 0) & 0xFFF;
            int frameVal = (tmpFrameDataBuffer >> 12) & 0xF; // instead of masking by 0xFFFFF... see step 2

            eyeFrameValBuffer = static_cast<float>(frameVal) / 0xF;
            eyeFrameValBuffer *= 5.0;

            Process::WriteFloat(writerCurrAddress, static_cast<float>(frameNum));
            Process::WriteFloat(writerCurrAddress + sizeof(u32), eyeFrameValBuffer);

            currentEyeAnimAddress += sizeof(u32);
            writerCurrAddress += sizeof(u64);
        }

        return true;
    }

    bool FaceExprEditor::copyMayuAnimData(u32 LFC_MA_masterStartAddr)
    {
        u16 mayuOffset = 0xFC;
        u32 writerCurrAddress = AddressList::getAddress("CustomAnimData") + (sizeof(u64) * frameCount) + animMetadataSize;
        u32 mayuCurrAddress = LFC_MA_masterStartAddr + mayuOffset;

        writeCategoryAnimMetadata(writerCurrAddress, 1);
        writerCurrAddress += animMetadataSize;

        Process::CopyMemory((void *)writerCurrAddress, (void *)mayuCurrAddress, sizeof(u64) * 3);
        mayuCurrAddress += sizeof(u64) * 3;
        writerCurrAddress += sizeof(u64) * 3;

        // no data exists for Frame 3; fill in missing data manually
        Process::WriteFloat(writerCurrAddress, 3.0);
        Process::WriteFloat(writerCurrAddress + sizeof(u32), 7.0);
        writerCurrAddress += sizeof(u64); // StepLinear64 single frame data = two u32

        Process::CopyMemory((void *)writerCurrAddress, (void *)mayuCurrAddress, sizeof(u64) * 8);

        return true;
    }

    bool FaceExprEditor::copyMouthAnimData(u32 LFC_MA_masterStartAddr)
    {
        u16 mouthOffset = 0x178;
        u32 writerCurrAddress = AddressList::getAddress("CustomAnimData") + (sizeof(u64) * frameCount * 2) + (animMetadataSize * 2);
        u32 mouthStartAddress = LFC_MA_masterStartAddr + mouthOffset;

        writeCategoryAnimMetadata(writerCurrAddress, 2);
        writerCurrAddress += animMetadataSize;

        Process::CopyMemory((void *)writerCurrAddress, (void *)mouthStartAddress, sizeof(u64) * 4);
        mouthStartAddress += sizeof(u64) * 4;
        writerCurrAddress += sizeof(u64) * 4;

        // no data exists for Frames 4-5; fill in missing data manually
        Process::WriteFloat(writerCurrAddress, 4.0);
        Process::WriteFloat(writerCurrAddress + sizeof(u32), 12.0);
        Process::WriteFloat(writerCurrAddress + sizeof(u32) * 2, 5.0);
        Process::WriteFloat(writerCurrAddress + sizeof(u32) * 3, 12.0);
        writerCurrAddress += sizeof(u64) * 2;

        Process::CopyMemory((void *)writerCurrAddress, (void *)mouthStartAddress, sizeof(u64) * 6);
        return true;
    }

    // Reference: https://github.com/gdkchan/SPICA/blob/master/SPICA/Formats/CtrH3D/Animation/H3DFloatKeyFrameGroup.cs
    void FaceExprEditor::writeCategoryAnimMetadata(u32 writerStartAddr, int curveIndex)
    {
        float startFrame = 0.0,
            endFrame = 11.0,
            valueScale = 1.0,
            valueOffset = 0.0,
            frameScale = 1.0,
            invDuration = 0.0909;

        u8 stepLinear64 = 0x6,
           interpolation = 0x0,
           prePostRepeat = 0x0;

        u32 writerAddr = writerStartAddr;

        Process::WriteFloat(writerAddr, startFrame);
        Process::WriteFloat(writerAddr + sizeof(u32), endFrame);
        writerAddr += sizeof(u32) * 2;

        Process::Write8(writerAddr, prePostRepeat);
        Process::Write8(writerAddr + sizeof(u8), prePostRepeat);
        Process::Write16(writerAddr + sizeof(u16), static_cast<u16>(curveIndex));
        writerAddr += sizeof(u32);

        Process::Write8(writerAddr, interpolation);
        Process::Write8(writerAddr + sizeof(u8), stepLinear64);
        Process::Write16(writerAddr + sizeof(u16), static_cast<u16>(frameCount));
        writerAddr += sizeof(u32);

        Process::WriteFloat(writerAddr, valueScale);
        Process::WriteFloat(writerAddr + sizeof(u32), valueOffset);
        Process::WriteFloat(writerAddr + sizeof(u32) * 2, frameScale);
        Process::WriteFloat(writerAddr + sizeof(u32) * 3, invDuration);
        writerAddr += sizeof(u32) * 4;

        Process::Write32(writerAddr, writerStartAddr + animMetadataSize); // ptr to frame data
    }

    bool FaceExprEditor::buildTexRefPtrBlock(void)
    {
        // TODO: could grab these values from master block instead of hardcoding...
        std::array<u16, newTexCount> masterListRelativeOffsets = { 0x27D, 0x289, 0x28F, 0x29B, 0x2A1, 0x2A7, 0x2FE, 0x305, 0x30C, 0x313, 0x31A, 0x32E, 0x336, 0x33E, 0x346, 0x34E, 0x2BE, 0x283, 0x295, 0x356 };

        u32 writerCurrAddress = AddressList::getAddress("CustomFilenamePtrList");
        u32 baseAddr = AddressList::getAddress("Link_anm_mat_MasterAssetListStart");

        for (u16 offset : masterListRelativeOffsets)
        {
            if (!Process::Write32(writerCurrAddress, baseAddr + offset))
            {
                OSD::Notify("[ERROR] Building texture reference pointer block failed.", Color::Red);
                return false;
            }

            writerCurrAddress += sizeof(u32);
        }
        return true;
    }

    /* -------------- EXECUTED DURING LOADING SEQUENCES -------------- */

    // Retrieves the dynamic starting addresses for each LFC_MA child block (4 total: Hytopia, G, B, R) by traversing known pointer chains
    void FaceExprEditor::editChild_FC_MA_Blocks(void)
    {
        // LFC_MA = Link_FaceChange Material Animation
        u8 finalLFC_MA_offset = 0x18;
        u8 LFC_MA_offsets[4] = {0x18, 0xB4, 0x6C, 0x4C};
        u32 LFC_MA_start = AddressList::getAddress("IndivLFC_MA_Start");
        u32 tmpPointerBuffer = 0x0;

        std::vector<u32> addresses;

        // NOTE: pointer chain detailed in spreadsheet
        for (int iter = 0; iter < 4; iter++)
        {
            // last iteration -> hytopia
            if (iter == 3)
                LFC_MA_start = AddressList::getAddress("HytopiaLFC_MA_Start");

            Process::Read32(LFC_MA_start, tmpPointerBuffer);
            if (GeneralHelpers::isNullPointer(tmpPointerBuffer))
            {
                OSD::Notify("[ERROR] LFC_MA start address is null.", Color::Red);
                break;
            }

            for (int jump = 0; jump < 4; jump++)
            {
                // update buffer with current pointer address
                Process::Read32(tmpPointerBuffer + LFC_MA_offsets[jump], tmpPointerBuffer);

                if (GeneralHelpers::isNullPointer(tmpPointerBuffer))
                    return;
            }

            copyRedirIH_PtrBlock(tmpPointerBuffer + finalLFC_MA_offset);
            redirCH(tmpPointerBuffer + finalLFC_MA_offset);
            redirTexRefPtr(tmpPointerBuffer + finalLFC_MA_offset);

            addresses.push_back(tmpPointerBuffer + finalLFC_MA_offset);
            LFC_MA_start += 4; // to retrieve adjacent address for B/R
        }
    }

    bool FaceExprEditor::copyRedirIH_PtrBlock(u32 LFC_MA_startAddr)
    {
        u8 IH_entrySize = 0x20;
        u16 IH_Offset = 0x3D3C, IH_SizeOffset = 0x4;
        u32 readerCurrAddress = LFC_MA_startAddr + IH_Offset;
        u32 writerCurrAddress = AddressList::getAddress("CustomInfoHeaderPtrs");
        u32 pointerBase = 0x0;

        Process::Read32(readerCurrAddress, readerCurrAddress);
        if (GeneralHelpers::isNullPointer(readerCurrAddress))
        {
            OSD::Notify("[ERROR] IH block pointer is null.", Color::Red);
            return false;
        }

        Process::CopyMemory((void *)writerCurrAddress, (void *)readerCurrAddress, sizeof(u32) * originalTexCount);
        writerCurrAddress += sizeof(u32) * originalTexCount;

        // derive missing pointers manually
        Process::Read32(readerCurrAddress, pointerBase); // this is eye.0 = tex index 0
        if (GeneralHelpers::isNullPointer(readerCurrAddress))
        {
            OSD::Notify("[ERROR] Missing IH pointers cannot be calculated.", Color::Red);
            return false;
        }

        Process::Write32(writerCurrAddress, pointerBase + IH_entrySize); // this is eye.1 = tex index 1
        Process::Write32(writerCurrAddress + sizeof(u32), pointerBase + IH_entrySize * 2); // this is eye.2 = tex index 2
        Process::Write32(writerCurrAddress + sizeof(u32) * 2, pointerBase + IH_entrySize * 5); // this is eye.5 = tex index 5
        Process::Write32(writerCurrAddress + sizeof(u32) * 3, pointerBase + IH_entrySize * 23); // this is mouth.5 = tex index 24 (not 19 due to main.0 - main.3)

        // update IH location + size references
        Process::Write32(LFC_MA_startAddr + IH_Offset, AddressList::getAddress("CustomInfoHeaderPtrs"));
        Process::Write32(LFC_MA_startAddr + IH_Offset + IH_SizeOffset, newTexCount);

        return true;
    }

    // Redirects Category Header reference pointers in the LFC_MA child blocks to the custom copy previously created in padding
    bool FaceExprEditor::redirCH(u32 LFC_MA_startAddr)
    {
        u8 CH_ptrOffset = 0xC;
        return (Process::Write32(LFC_MA_startAddr - CH_ptrOffset, AddressList::getAddress("RedirCategoryHeaderPtrs")));
    }

    // Redirects texture filename reference pointer list in the LFC_MA child blocks to the custom copy previously created in padding
    bool FaceExprEditor::redirTexRefPtr(u32 LFC_MA_startAddr)
    {
        u8 texPtrBlockSizeOffset = 0x4;

        Process::Write32(LFC_MA_startAddr + texPtrBlockSizeOffset, newTexCount);
        return (Process::Write32(LFC_MA_startAddr, AddressList::getAddress("CustomFilenamePtrList")));
    }

    /* -------------- EXECUTED ONCE PER EDIT TRIGGER -------------- */

    // Updates frame data for eye, eyebrow (mayu), and mouth
    void FaceExprEditor::updateAnimData(void)
    {
        const std::vector<IconLabel> infoArrays[] = {eyeInfo, mayuInfo, mouthInfo};

        u8 faceSectionSize = 0x60 + animMetadataSize;
        u32 startAddr = AddressList::getAddress("CustomAnimData") + animMetadataSize;
        u32 entryOffset = _frameIndex * sizeof(u64); // each entry contains two floats: frameNum and frameVal

        for (int iter = 0; iter < 3; iter++)
        {
            // frameNum already exists in the duplicated data block -> only frameVal edited here
            Process::WriteFloat(startAddr + (faceSectionSize * iter) + entryOffset + sizeof(u32), infoArrays[iter][currExprIndexes[iter]].floatVal);
        }
    }

    /* -------------- EXECUTED ONCE UPON ENTRY DISABLE -------------- */

    // Reverts edits made to the Master LFC_MA texture filename reference pointer block
    // Note: this automatically propagates to the LFC_MA child blocks
    // bool FaceExprEditor::restoreMasterTexRefBlock(void)
    // {

    // }

    /* -------------- DRIVER CODE -------------- */

    // Edits are maintained by redirecting child LFC_MA block pointers (for CH, IH, and texture filename reference blocks) to custom data placed in padding
    // Reverting back to original data is done simply by stopping the redirection process and letting the game use its default pointers instead
    void FaceExprEditor::editMngr(MenuEntry* entry)
    {
        u32 noInterruptEdit, noIntrEditOffset = 0x60;
        u32 LFC_MA_masterStartAddr = AddressList::getAddress("LFC_MA_MasterStart");

        // if one phase fails, don't continue:
        if (entry->WasJustActivated())
        {
            if (!FaceExprEditor::restoreEyeTexMasterList(LFC_MA_masterStartAddr))
                goto error;
            if (!FaceExprEditor::expandMasterTexRefBlock(LFC_MA_masterStartAddr))
                goto error;
            if (!FaceExprEditor::copyEyeAnimData(LFC_MA_masterStartAddr))
                goto error;
            if (!FaceExprEditor::copyMayuAnimData(LFC_MA_masterStartAddr))
                goto error;
            if (!FaceExprEditor::copyMouthAnimData(LFC_MA_masterStartAddr))
                goto error;
            if (!FaceExprEditor::initCustomCH(LFC_MA_masterStartAddr))
                goto error;
            if (!FaceExprEditor::buildTexRefPtrBlock())
                goto error;
        }

        // if noInterruptEdit is null, the game is currently rearranging dynamic memory during loading sequences
        // ...interrupting this edit sequence will lead to a crash
        if (GeneralHelpers::isLoadingScreen(false))
        {
            Process::Read32(AddressList::getAddress("IndivLFC_MA_Start") + noIntrEditOffset, noInterruptEdit);
            if (!GeneralHelpers::isNullPointer(noInterruptEdit))
            {
                FaceExprEditor::editChild_FC_MA_Blocks();
            }
        }
        return;

        error:
            OSD::Notify("Facial Expression Editor: An error occurred during init.", Color::Yellow);
            OSD::Notify("Edits have not been saved.", Color::Yellow);
            entry->Disable();
    }

    /* -------------- UI -------------- */

}