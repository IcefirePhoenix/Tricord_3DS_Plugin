#include "AddressList.hpp"
#include "Helpers.hpp"

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
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

    // can/should preset costumes be provided
    // faux hero, full zora (get sharing perms first), etc.
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

    const StringVector GameData::itemList = {
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

    const StringVector GameData::worldList = {
        "Woodlands",
        "Riverside",
        "Volcano",
        "Ice Cavern",
        "Fortress",
        "The Dunes",
        "The Ruins",
        "Sky Realm"
    };

    const StringVector GameData::woodlandLevelList = {
        "Deku Forest",
        "Buzz Blob Cave",
        "Moblin Base",
        "Forest Temple"
    };

    const StringVector GameData::riversideLevelList = {
        "Secret Fortress",
        "Abyss of Agony",
        "Cove of Transition",
        "Water Temple"
    };

    const StringVector GameData::volcanoLevelList = {
        "Blazing Trail",
        "Hinox Mine",
        "Den of Flames",
        "Fire Temple"
    };

    const StringVector GameData::iceLevelList = {
        "Frozen Plateau",
        "Snowball Ravine",
        "Silver Shrine",
        "Ice Temple"
    };

    const StringVector GameData::fortressLevelList = {
        "Sealed Gateway",
        "Bomb Storage",
        "Training Ground",
        "The Lady's Lair"
    };

    const StringVector GameData::dunesLevelList = {
        "Infinity Dunes",
        "Stone Corridors",
        "Gibdo Mausoleum",
        "Desert Temple"
    };

    const StringVector GameData::ruinsLevelList = {
        "Illusory Mansion",
        "Palace Noir",
        "Gibdo Mausoleum",
        "Grim Temple"
    };

    const StringVector GameData::skyLevelList = {
        "Floating Garden",
        "Deception Castle",
        "Dragon Citadel",
        "Sky Temple"
    };

    const StringVector GameData::challengeList = {
        "No challenge",
        "Challenge 1",
        "Challenge 2",
        "Challenge 3"
    };


    const StringVector GameData::lobbyBallSongs = {
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

    const StringVector GameData::voiceList = {
        "A Link Between Worlds",
        "Wind Waker",
        "Ocarina of Time",
        "Spirit Tracks"
    };


    const std::vector<u8> GameData::emoteIDs = {
        0x00,  // over here
        0x01,  // item
        0x02,  // throw
        0x03,  // totem
        0x04,  // cheer
        0x05,  // frown (apologetic bow in JP/KOR)
        0x06,  // nooo
        0x07,  // thumbs
        0x08,  // hello
        0x09,  // yawn
        0x0A,  // let's go
        0x0B,  // waving
        0x0C   // blank
    };

    const int GameData::generalPlayerIDs[3] = {0, 1, 2};
    const u32 GameData::playerAddressOffset = 0x10000;

    int GameData::getPlayerIDFromColor(std::string color) 
    {
        if (color == "Green")
            return 0;
        else if (color == "Blue")
            return 1;
        else if (color == "Red")
            return 2;
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

    std::string GameData::getCostumeNameFromID(int ID) 
    {
        if (ID < 0x26)
            return GameData::universalCostumeList[ID];
        else if (ID < 0x30)
            return GameData::customCostumeList[ID - 0x26];
        else
            return "N/A";
    }    
    
    StringVector GameData::getWorldNamesfromID(int ID) 
    {
        switch (ID) 
        {
        case 0: 
            return GameData::woodlandLevelList;
        case 1:
            return GameData::riversideLevelList;
        case 2:
            return GameData::volcanoLevelList;
        case 3:
            return GameData::iceLevelList;
        case 4:
            return GameData::fortressLevelList;
        case 5:
            return GameData::dunesLevelList;
        case 6:
            return GameData::ruinsLevelList;
        default:
            return GameData::skyLevelList;
        }
    }

    int GameData::selWorld(bool useDoT) 
    {
        StringVector worldSelectionList = GameData::worldList;

        if (useDoT)
            worldSelectionList.push_back("Den of Trials");

        Keyboard chooseWorld("Select a world:");
        chooseWorld.Populate(worldSelectionList);

        return chooseWorld.Open();
    }

    std::string GameData::worldIDToStr(int worldID) 
    {
        return GameData::worldList[worldID];
    }

    std::string GameData::getVoiceAsStr(u8 ID) 
    {
        return GameData::voiceList[ID];
    }

    u32 GameData::getLobbyBallDataAddress(void)
    {
        u32 address;
        Process::Read32(AddressList::LobbyBallPointer.addr, address);

        return (address);
    }
}