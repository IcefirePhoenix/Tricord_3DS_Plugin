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

    // can/should preset costumes be provided
    // faux hero, full zora (get sharing perms first), etc.
    extern const StringVector customCostumeList =
    {
        "Custom Costume 1",
        "Custom Costume 2",
        "Custom Costume 3",
        "Custom Costume 4"
    };

    extern const StringVector swordList =
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
        "Fierce Deity Armor sword",
        "No sword",
        "Fire Glove - dev/debug"
    };
   
    extern const StringVector itemList = {
        "Bombs",
        "Bow",
        "Fire Gloves",
        "Boomerang",
        "Water Rod",
        "Gust Jar",
        "Gripshot",
        "Hammer"
        "No item"
    };

    extern const StringVector otherMats = {
        "Lady's Glasses",
        "Lady's Collar",
        "Lady's Parasol",
        "Frilly Fabric",
        "Friendly Token",
        "Freebie"
    };

    extern const StringVector woodlandMats = {
        "Blob Jelly",
        "Armos Spirit",
        "Tektite Shell",
        "Sweet Shroom",
        "Blin Bling",
        "Gohma's Eye",
        "Mystery Jade"
        "Supple Leaf"
    };

    extern const StringVector riversideMats = {
        "Fresh Kelp",
        "Zora Scale",
        "Hytopia Silk",
        "Aqua Crown",
        "Octorok Suckers",
        "Fairy Dust",
        "Rainbow Coral",
        "Thornberry"
    };

    extern const StringVector volcanoMats = {
        "Goron Ore",
        "Monster Guts",
        "Demon Fossil",
        "Rugged Horn",
        "Kodongo Tail",
        "Keese Wing",
        "Star Fragment",
        "Divine Whiskers"
    };

    extern const StringVector iceMats = {
        "Fluffy Fuzz",
        "Tiny Snowflake",
        "Serpent Fangs",
        "Crimson Shell",
        "Freezard Water",
        "Chill Stone",
        "Ice Rose",
        "Beastie Patch",
    };

    extern const StringVector fortressMats = {
        "Stiff Puff",
        "Silver Thread",
        "Royal Ring",
        "Ancient Coin",
        "Fancy Fabric",
        "Exquisite Lace",
        "Vibrant Brooch"
        "Brittle Papyrus"
    };

    extern const StringVector dunesMats = {
        "Palm Cone",
        "Ancient Fin",
        "Vintage Linen",
        "Gibdo Bandage",
        "Stal Skull",
        "Sandy Ribbon",
        "Crystal Skull",
        "Golden Insect"
    };

    extern const StringVector ruinsMats = {
        "Carrumpkin",
        "Mystery Extract",
        "Spider Silk Lace",
        "Poe Soul",
        "Twisted Twig",
        "Lava Drop",
        "Sanctuary Mask",
        "Gold Dust" 
    };

    extern const StringVector skyMats = {
        "Cucco Feathers",
        "Carmine Pearl",
        "Sky Dragon Tail",
        "Pretty Plume",
        "Mock Fairy",
        "Aurora Stone",
        "Steel Mask",
        "Fabled Butterfly"
    };

    extern const StringVector lobbyBallSongs = {
        "Ballad of the Goddess (SS)",
        "Epona's Song (OoT)",
        "Clock Town (MM)"
        "Saria's Song/Lost Woods (OoT)",
        "Overworld (ALttP)"
        "Dark World (ALttP)",
        "Song of Storms (OoT)",
        "Fairy Fountain (OoT)",
        "Princess Zelda's Theme (OoT)",
        "Hyrule Field (TP)",
        "Full Steam Ahead (ST)",
        "Skyloft (SS)",
        "Follow Fi (SS)",
        "Title Theme (WW)"
        "Mother Maiamai (ALBW)"
        "Princess Hilda's Theme (ALBW)",
        "Yuga Battle (ALBW)",
        "Lorule Castle (ALBW)",
        "Linebeck's Theme (PH)"
        "Title Theme (TFH)",
        "Madame Couture (TFH)",
        "Ballad of the Wind Fish (LA)",
        "The Great Sea (PH)",
        "Nothing"
    };

    static const std::vector<u8> emoteIDs = {
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


    u32 chooseLink(void) {
        Keyboard player("Choose a Link:");
        // Populate the bottom screen with options:
        static const StringVector linkList =
        {
            "Player 1 (Green)",
            "Player 2 (Blue)",
            "Player 3 (Red)"
        };
        player.Populate(linkList);

        u32 result = player.Open();
        return result;
    }

    // borrowed from OnionFS
    bool showMsgKbd(std::string text, DialogType digtype) {
        Keyboard kbd(text);
        StringVector opts;
        switch (digtype)
        {
        case CTRPluginFramework::DialogType::DialogOk:
            opts = { "Ok" };
            break;
        case CTRPluginFramework::DialogType::DialogOkCancel:
            opts = { "Ok", "Cancel" };
            break;
        case CTRPluginFramework::DialogType::DialogYesNo:
            opts = { "Yes", "No" };
            break;
        default:
            break;
        }
        kbd.Populate(opts);
        return kbd.Open() == 0;
    }

}
