#include "Helpers.hpp"

namespace CTRPluginFramework
{
    /* loosely based on HotkeyModifier class */

    u8 backupFirstBitSet[4];
    u32 bitstringLocations[3][4];
    u64 bitstring[4]; // array elements: noChallenge, challenge1, challenge2, challenge3

    LevelStatusEditor::LevelStatusEditor(const std::string &message, StringVector levelNames, int world, int playMode) :
        _message(message), _levelNames(levelNames), _world(world), _playMode(playMode)
    {
        int posY = 100;
        for (int rowNum = 0; rowNum < 4; rowNum++)
        {
            int posX = 180;
            for (int columnNum = 0; columnNum < 4; columnNum++)
            {
                Button newButton(Button::Icon | Button::Toggle, IntRect(posX, posY, 20, 20), Icon::DrawCheckBox);
                _checkboxes.push_back(newButton);

                posX += 30;
            }
            posY += 32;
        }

        initBitstringAddresses();
        setCheckboxes();
    }

    LevelStatusEditor::~LevelStatusEditor()
    {
        for (int index = 0; index < 16; index++)
        {
            int rowNum = index / 4;
            int columnNum = index % 4;
            int editedBit = getBit(rowNum);

            if (_checkboxes[index].GetState())
                bitstring[columnNum] |= (1 << editedBit);
            else
                bitstring[columnNum] &= ~(1 << editedBit);
        }

        // addresses should only be updated one time -> once the menu is closed
        // attempting to edit memory in real-time while the menu is still open -> multiple, concurrent edits -> corrupted edits
        for (int bitstringIterator = 0; bitstringIterator < 4; bitstringIterator++)
        {
            writeUpdates(bitstringIterator);
        }
    }

    void LevelStatusEditor::initBitstringAddresses(void)
    {
        Address bitstringArray[3][4] =
        {
            {AddressList::SoloNCCompletion, AddressList::SoloC1Completion, AddressList::SoloC2Completion, AddressList::SoloC3Completion},
            {AddressList::MultiNCCompletion, AddressList::MultiC1Completion, AddressList::MultiC2Completion, AddressList::MultiC3Completion},
            {AddressList::NormalNCCompletion, AddressList::NormalC1Completion, AddressList::NormalC2Completion, AddressList::NormalC3Completion}
        };

        for (int modeIndex = 0; modeIndex < 3; modeIndex++)
        {
            for (int levelIndex = 0; levelIndex < 4; levelIndex++)
                bitstringLocations[modeIndex][levelIndex] = bitstringArray[modeIndex][levelIndex].addr;
        }
    }

    void LevelStatusEditor::copyBitstrings(int modeIndex)
    {
        for (int bitstringIterator = 0; bitstringIterator < 4; bitstringIterator++)
        {
            Process::Read64(bitstringLocations[modeIndex][bitstringIterator], bitstring[bitstringIterator]);

            // right-shift to remove unknown-purpose first bit (but save it somewhere beforehand...)
            backupFirstBitSet[bitstringIterator] = bitstring[bitstringIterator] & 1;
            bitstring[bitstringIterator] = bitstring[bitstringIterator] >> 1;
        }
    }

    void LevelStatusEditor::setCheckboxes()
    {
        int minimumLevelIndex = 0;
        int maxLevelIndex = 4;
        int offset = _world * 4; // each world takes up 4 bits = 4 levels

        copyBitstrings(_playMode);

        // reflect current bitstring statuses onto checkboxes during init
        for (int iterateThruLevels = minimumLevelIndex + offset; iterateThruLevels < maxLevelIndex + offset; iterateThruLevels++)
        {
            for (int iterateThruChal = 0; iterateThruChal < 4; iterateThruChal++)
            {
                int currRow = iterateThruLevels - offset;
                int currCol = iterateThruChal;

                int index = (currRow * 4) + currCol;

                if (bitstring[iterateThruChal] & (1 << iterateThruLevels))
                    _checkboxes[index].SetState(true);
            }
        }
    }

    u32     LevelStatusEditor::getBitstringAddress(int bitstringID, int playMode)
    {
        return bitstringLocations[playMode][bitstringID];
    }

    void    LevelStatusEditor::operator()(void)
    {
        bool mustclose = false;
        bool sleepClose = false;

        while (((!Window::BottomWindow.MustClose() && !mustclose)) && !sleepClose)
        {
            Controller::Update();
            mustclose = Controller::IsKeyPressed(Key::B);
            sleepClose = SystemImpl::IsSleeping();

            _DrawTop();
            _DrawBottom();

            Renderer::EndFrame();
            _Update();
        }
    }

    // bitwise operation requires _playMode to be 1-indexed -> saved to new var editQueue:
    // editQueue = 1 -> solo edits only (1&1 = true)
    // editQueue = 2 -> multi edits only (2&2 = true)
    // editQueue = 3 -> both edited (3&1 = true and 3&2 = true)
    void    LevelStatusEditor::writeUpdates(int bitstringID)
    {
        u64 finalMultiProgress, finalSoloProgress;
        int editQueue = _playMode + 1;

        bitstring[bitstringID] = bitstring[bitstringID] << 1;
        bitstring[bitstringID] |= backupFirstBitSet[bitstringID];

        if (editQueue & 1)
            Process::Write64(getBitstringAddress(bitstringID, 0), bitstring[bitstringID]);

        if (editQueue & 2)
            Process::Write64(getBitstringAddress(bitstringID, 1), bitstring[bitstringID]);

        // regardless of edits above, update the *general* completion board (solo and multi progress combined via AND)
        Process::Read64(getBitstringAddress(bitstringID, 0), finalSoloProgress);
        Process::Read64(getBitstringAddress(bitstringID, 1), finalMultiProgress);
        Process::Write64(getBitstringAddress(bitstringID, _playMode), finalSoloProgress & finalMultiProgress);
    }

    int     LevelStatusEditor::getBit(int rowNum)
    {
        return (_world * 4) + rowNum;
    }

    void    LevelStatusEditor::_DrawTop(void) const
    {
        Renderer::SetTarget(TOP);
        Window::TopWindow.Draw("Level Completion Editor");

        int posY = 61;
        Renderer::DrawSysStringReturn((const u8*)_message.c_str(), 40, posY, 335, Preferences::Settings.MainTextColor);
    }

    const StringVector labels =
    {
        "NC",
        "C1",
        "C2",
        "C3"
    };

    void    LevelStatusEditor::_DrawBottom(void)
    {
        Renderer::SetTarget(BOTTOM);
        Window::BottomWindow.Draw();

        // draw checkboxes
        for (auto it = _checkboxes.begin(); it != _checkboxes.end(); it++)
            (*it).Draw();

        // draw labels
        /* Renderer::DrawSysString auto-increments posY param by 16 -> this is done to keep menu entries spaced correctly
        However, we want posY to remain unchanged... lazy workaround below -> no need to rework the framework library this way */

        int yPositions[4];
        std::fill(yPositions, yPositions + 4, 80);

        Renderer::DrawSysString(labels[0].c_str(), 178, yPositions[0], 290, Preferences::Settings.MainTextColor);
        Renderer::DrawSysString(labels[1].c_str(), 208, yPositions[1], 290, Preferences::Settings.MainTextColor);
        Renderer::DrawSysString(labels[2].c_str(), 238, yPositions[2], 290, Preferences::Settings.MainTextColor);
        Renderer::DrawSysString(labels[3].c_str(), 268, yPositions[3], 290, Preferences::Settings.MainTextColor);

        for (int i = 0, lvlY = 100; i < 4; i++, lvlY += 15)
            Renderer::DrawSysString(_levelNames[i].c_str(), 38, lvlY, 290, Preferences::Settings.MainTextColor);
    }

    void    LevelStatusEditor::_Update(void)
    {
        bool        isTouched = Touch::IsDown();
        IntVector   touchPos(Touch::GetPosition());

        for (auto it = _checkboxes.begin(); it != _checkboxes.end(); it++)
            (*it).Update(isTouched, touchPos);

        Window::BottomWindow.Update(isTouched, touchPos);
    }

    void    LevelStatusEditor::_EditDoTCompletion(void)
    {
        // TODO:
        // no challenge only = bitstring[0]
        // design new menu with 8 checkboxes only
    }
}
