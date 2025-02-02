#include "Helpers.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    // Removes player voice
    void Save::useVoiceless(MenuEntry *entry)
    {
        Process::Write8(AddressList::MainVoice.addr, 0x4);
    }

    // Changes player voice (randomly generated once during savefile creation and was not intended to be edited)
    void Save::mainVoice(MenuEntry *entry)
    {
        std::string menuIntro = "Choose a voice:\n\nThese changes can be observable online. Keep in mind your selection will be overwritten by Voiceless, if used.";

        Keyboard voiceMenu(menuIntro);
        voiceMenu.Populate(GameData::voiceList);
        int result = voiceMenu.Open();

        if (result >= 0)
        {
            Process::Write8(AddressList::MainVoice.addr, result);
            entry->SetName("Set Main Voice: " + GameData::getVoiceAsStr(result));
        }
    }
}