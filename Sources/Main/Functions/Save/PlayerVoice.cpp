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
        Keyboard voiceMenu("Choose a voice:\n\nThese changes can be observable online. Keep in\nmind your selection will be overwritten by Voiceless,\nif used.");
        voiceMenu.Populate(GameData::voiceList);

        int result = voiceMenu.Open();

        if (result >= 0)
        {
            Process::Write8(AddressList::MainVoice.addr, static_cast<u8>(result));
            entry->SetName("Set Main Voice: " + GameData::getVoiceAsStr(result));
        }
    }
}