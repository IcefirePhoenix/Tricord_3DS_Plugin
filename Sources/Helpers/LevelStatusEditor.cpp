#include "Helpers/LevelStatusEditor.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Icon.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Renderer.hpp"
#include "CTRPluginFrameworkImpl/Graphics/Window.hpp"
#include "CTRPluginFramework/System/Controller.hpp"
#include "Unicode.h"
#include "CTRPluginFramework/System/System.hpp"
#include "CTRPluginFrameworkImpl/System/SystemImpl.hpp"
#include "CTRPluginFrameworkImpl/Preferences.hpp"
#include "CTRPluginFramework/Sound.hpp"
#include "cheats.hpp"


namespace CTRPluginFramework
{
    /* loosely based on HotkeyModifier class */

    const u32 bitstringLocations[3][4] = {
        AddressList::SoloNCCompletion.addr, AddressList::SoloC1Completion.addr, AddressList::SoloC2Completion.addr, AddressList::SoloC3Completion.addr,
        AddressList::MultiNCCompletion.addr, AddressList::MultiC1Completion.addr, AddressList::MultiC2Completion.addr, AddressList::MultiC3Completion.addr,
        AddressList::NormalNCCompletion.addr, AddressList::NormalC1Completion.addr, AddressList::NormalC2Completion.addr, AddressList::NormalC3Completion.addr
    };

    LevelStatusEditor::LevelStatusEditor(const std::string &message, StringVector levelNames, int world, int playMode) :
        _message(message), _levelNames(levelNames), _world(world), _playMode(playMode)
    {
        for (int iterateLvl1 = 0, posY = 100; iterateLvl1 < 4; ++iterateLvl1, posY += 32)
        {
            Button newButton(Button::Icon | Button::Toggle, IntRect(180, posY, 20, 20), Icon::DrawCheckBox);
            _checkboxes.push_back(newButton);
        }
        for (int iterateLvl2 = 0, posY = 100; iterateLvl2 < 4; ++iterateLvl2, posY += 32)
        {
            Button newButton(Button::Icon | Button::Toggle, IntRect(210, posY, 20, 20), Icon::DrawCheckBox);
            _checkboxes.push_back(newButton);
        }
        for (int iterateLvl3 = 0, posY = 100; iterateLvl3 < 4; ++iterateLvl3, posY += 32)
        {
            Button newButton(Button::Icon | Button::Toggle, IntRect(240, posY, 20, 20), Icon::DrawCheckBox);
            _checkboxes.push_back(newButton);
        }
        for (int iterateLvl4 = 0, posY = 100; iterateLvl4 < 4; ++iterateLvl4, posY += 32)
        {
            Button newButton(Button::Icon | Button::Toggle, IntRect(270, posY, 20, 20), Icon::DrawCheckBox);
            _checkboxes.push_back(newButton);
        }

        setCheckboxes(world);
    }

    LevelStatusEditor::~LevelStatusEditor()
    {
    }

    u64 challenge[4]; // array elements: noChallenge, challenge1, challenge2, challenge3

    void LevelStatusEditor::copyBitstrings(int playMode) {
        for (int iterator = 0; iterator < 4; ++iterator) {
            challenge[iterator] = bitstringLocations[playMode][iterator];
        }
    }

    void LevelStatusEditor::setCheckboxes(int world) {
        int lowRange = 1;
        int highRange = 4;
        int offset = world * 4; // each world takes up 4 bits = 4 levels

        copyBitstrings(_playMode);

        // reflect current bitstring statuses onto checkboxes during init
        for (int iterateThruLevels = lowRange + offset; iterateThruLevels < highRange + offset; ++iterateThruLevels) {
            for (int iterateThruChal = 0; iterateThruChal < 4; ++iterateThruChal) {
                int currRow = iterateThruLevels - (lowRange + offset) + 1;
                int currCol = iterateThruChal + 1;

                int index = (currRow * 4) + currCol;

                if (challenge[iterateThruChal] & (1u << iterateThruLevels))
                    _checkboxes[index].SetState(true);
            }
        }
    }

    u32     LevelStatusEditor::getBitstringAddress(int bitstringID, int playMode) {
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

            // 16 total boxes, loop through columns->rows
            for (int challengeCol = 0; challengeCol < 4; ++challengeCol) {
                for (int levelRow = 0; levelRow < 4; ++levelRow) {
                    int index = challengeCol * 4 + levelRow;

                    if (_checkboxes[index].GetState()) {
                        int bit = getBit(index, levelRow);
                        challenge[challengeCol] |= (1 << bit);
                    }
                    writeUpdates(challengeCol);
                }
            }
        }
    }

    void    LevelStatusEditor::writeUpdates(int bitstringID) {
        if (_playMode == 0)  // solo
            Process::Write64(getBitstringAddress(bitstringID, 0), challenge[bitstringID]);
        else if (_playMode == 1 )// multi
            Process::Write64(getBitstringAddress(bitstringID, 1), challenge[bitstringID]);

        // general completion
        Process::Write64(getBitstringAddress(bitstringID, 2), challenge[bitstringID]);
    }

    int     LevelStatusEditor::getBit(int index, int rowNum) {
        return (_world * 4) + rowNum;
    }

    void    LevelStatusEditor::_DrawTop(void) const
    {
        Renderer::SetTarget(TOP);
        Window::TopWindow.Draw("Level Completion Editor");

        int posY = 61;
        Renderer::DrawSysStringReturn((const u8*)_message.c_str(), 40, posY, 335, Preferences::Settings.MainTextColor);
    }

    const StringVector labels = {
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

        for (int i = 0, lvlY = 100; i < 4; ++i, lvlY += 15) {
            Renderer::DrawSysString(_levelNames[i].c_str(), 38, lvlY, 290, Preferences::Settings.MainTextColor);
        }
    }

    void    LevelStatusEditor::_Update(void)
    {
        bool        isTouched = Touch::IsDown();
        IntVector   touchPos(Touch::GetPosition());

        for (auto it = _checkboxes.begin(); it != _checkboxes.end(); it++)
            (*it).Update(isTouched, touchPos);

        Window::BottomWindow.Update(isTouched, touchPos);
    }
}
