#include "Helpers.hpp"

namespace CTRPluginFramework
{
    const StringVector GameData::universalCostumeList =
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

    const StringVector GameData::customCostumeList =
    {
        "Custom Costume 1",
        "Custom Costume 2",
        "Custom Costume 3",
        "Custom Costume 4"
    };

    const StringVector GameData::swordList =
    {
        "Default sword",
        "Spin Attack Attire sword",
        "Ninja Gi sword",
        "Sword Suit sword",
        "Sword Master Suit sword",
        "Dapper Spinner sword",
        "Lady's Ensemble sword",
        "Tri Suit sword",
        "Timeless Tunic sword",
        "Linebeck's Uniform sword",
        "Fierce Deity Armor sword",
        "No sword - crashes game",
        "Fire Glove - dev/debug"
    };

    const StringVector GameData::itemList =
    {
        "Bombs",
        "Bow",
        "Fire Gloves",
        "Boomerang",
        "Water Rod",
        "Gust Jar",
        "Gripshot",
        "Hammer",
        "No item"
    };

    const StringVector GameData::lobbyBallSongs =
    {
        "Ballad of the Goddess (SS)",
        "Epona's Song (OoT)",
        "Clock Town (MM)",
        "Saria's Song/Lost Woods (OoT)",
        "Overworld (ALttP)",
        "Dark World (ALttP)",
        "Song of Storms (OoT)",
        "Fairy Fountain (OoT)",
        "Princess Zelda's Theme (OoT)",
        "Hyrule Field (TP)",
        "Full Steam Ahead (ST)",
        "Skyloft (SS)",
        "Follow Fi (SS)",
        "Title Theme (WW)",
        "Mother Maiamai (ALBW)",
        "Princess Hilda's Theme (ALBW)",
        "Yuga Battle (ALBW)",
        "Lorule Castle (ALBW)",
        "Linebeck's Theme (PH)",
        "Title Theme (TFH)",
        "Madame Couture (TFH)",
        "Ballad of the Wind Fish (LA)",
        "The Great Sea (PH)",
        "Nothing"
    };

    const StringVector GameData::voiceList =
    {
        "A Link Between Worlds",
        "Wind Waker",
        "Ocarina of Time",
        "Spirit Tracks"
    };


    const std::vector<u8> GameData::emoteIDs =
    {
        0x00,  // over here
        0x01,  // item
        0x02,  // throw
        0x03,  // totem
        0x04,  // cheer
        0x05,  // frown (apologetic bow in JP)
        0x06,  // nooo
        0x07,  // thumbs
        0x08,  // hello
        0x09,  // yawn
        0x0A,  // let's go
        0x0B,  // waving
        0x0C   // blank
    };

    const std::unordered_map<u8, std::string> GameData::challengeList =
    {
        {0x0, "No challenge"},
        {0x1, "Transport the orb!"},
        {0x2, "Don't drop the pot!"},
        {0x3, "Transport the orb quickly!"},
        {0x4, "Guard the orb!"},
        {0x5, "Defeat all enemies!"},
        {0x6, "Pop all balloons!"},
        {0x7, "Don't pop any balloons!"},
        {0x8, "Adventure in the dark!"},
        {0x9, "Win without using a sword!"},
        {0xA, "Only Bombs -- no swords!"},
        {0xB, "Only Hammers -- no swords!"},
        {0xC, "Defeat all foes sans sword!"},
        {0xD, "Fewer Heart Containers!"},
        {0xE, "Clear within the time limit!"},
        {0xF, "Clear using only Bombs!"},
        {0x10, "Clear without any items!"},
        {0x11, "Don't fall at all!"},
        {0x12, "Clear with halved energy!"},
        {0x13, "Avoid the volcanic rocks!"},
        {0x14, "Avoid falling and quicksand!"},
        {0x15, "Evade the Wallmaster!"},
        {0x16, "Halved attack and defense!"},
        {0x17, "Don't get hit by snowballs!"}
    };

    // TODO: go through possible Actors...
    const std::unordered_map<std::string, std::string> GameData::actorNames =
    {
        {"EnemyAbyssBoneThrowStalfos", ""}
    };

    const int GameData::generalPlayerIDs[3] = { 0, 1, 2 };
    const int GameData::maxCostumeCount = 38;

    int GameData::getPlayerIDFromColor(std::string color)
    {
        if (color == "Green")
            return 0;
        else if (color == "Blue")
            return 1;
        else if (color == "Red")
            return 2;
        else if (color == "All")
            return 3;
        else
            return -1;
    }

    std::string GameData::getSwordNameFromID(int ID)
    {
        if (ID < 0xD)
            return GameData::swordList[ID];
        else
            return "N/A";
    }

    std::string GameData::getChallengeName(int chalID)
    {
        if (chalID < 0x18)
            return GameData::challengeList.at(chalID);
        else
            return "N/A";
    }

    std::string GameData::getCostumeNameFromID(int ID)
    {
        u8 unusedDLC_SlotCount = 0x4;

        if (ID < maxCostumeCount)
            return GameData::universalCostumeList[ID];
        else if (ID < maxCostumeCount + unusedDLC_SlotCount)
            return GameData::customCostumeList[ID - maxCostumeCount];
        else
            return "N/A";
    }

    int GameData::getCostumeIDFromName(std::string costumeName)
    {
        for (int iterator = 0; iterator < GameData::maxCostumeCount; iterator++)
        {
            if (universalCostumeList[iterator] == costumeName)
                return iterator;
        }
        return -1; // wasn't found
    }

    std::string GameData::getVoiceAsStr(u8 ID)
    {
        return GameData::voiceList[ID];
    }
}