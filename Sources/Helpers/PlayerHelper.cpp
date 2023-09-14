#include "cheats.hpp"
#include "csvc.h"
#include "AddressList.hpp"
#include "Helpers/Address.hpp"
#include "CTRPluginFramework/Menu/MessageBox.hpp"
#include "CTRPluginFramework/Graphics/OSD.hpp"

#include <CTRPluginFramework.hpp>

#include "3ds.h"

namespace CTRPluginFramework
{
    extern const StringVector universalCostumeList =
    {
        "Bear Minimum/Maximum",
        "Sword Suit",
        "Kokiri Clothes",
        "Legendary Dress",
        "Showstopper",
        "Goron Garb",
        "Zora Costume",
        "Queen of Hearts",
        "Big Bomb Outfit",
        "Energy Gear",
        "Cozy Parka",
        "Spin Attack Attire",
        "Ninja Gi",
        "Rupee Regalia",
        "Boomeranger",
        "Tri Suit",
        "Lucky Loungewear",
        "Torrent Robe",
        "Lady's Ensemble",
        "Cursed Tights",
        "Timeless Tunic",
        "Light Armor",
        "Cheetah Costume",
        "Jack of Hearts",
        "Hero's Tunic",
        "Gust Garb",
        "Hammerwear",
        "Dunewalker Duds",
        "Robowear",
        "Fire Blazer",
        "Tingle Tights",
        "Serpent's Toga",
        "Sword Master Suit",
        "Dapper Spinner",
        "Cheer Outfit",
        "Cacto Clothes",
        "Linebeck's Uniform",
        "Fierce Deity Armor"
    };

    // should we provide preset costumes for this too?
    // faux hero, full zora (ask mergie), etc.
    extern const StringVector customCostumeList =
    {
        "Custom Costume 1",
        "Custom Costume 2",
        "Custom Costume 3",
        "Custom Costume 4"
    };

    int chooseLink(void) {
        Keyboard player("Choose a Link:\ntest");
        // Populate the bottom screen with options:
        static const StringVector linkList =
        {
            "Player 1 (Green)",
            "Player 2 (Blue)",
            "Player 3 (Red)"
        };
        player.Populate(linkList);

        return player.Open();
    }
}
