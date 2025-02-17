#include "Helpers.hpp"
#include "Main/Costume.hpp"

#include <bitset>
#include "General/pIDindex.h"

namespace CTRPluginFramework
{
    MenuEntry* infBalloonsAuto;
    MenuEntry* dapperAuto;
    MenuEntry* beamCooldownAuto;

    bool beamStatuses[3] = {false, false, false};
    bool useBeamCooldown;

    std::bitset<42> indCostumeEffects[3];
    bool infTingleBalloons[3] = { false, false, false };
    bool dapperSpinA[3] = { false, false, false }; bool dapperSpinB[3] = { false, false, false };

    /* --------------------------------- */

    // Sets a status-bit-based costume effect for the given player and ID arguments
    // ID refers to the position of the status bit for the costume effect
    // TODO: Make sure the big sword beams code is removed from here
    void Costume::setIndCostumeEffect(MenuEntry* entry)
    {
        pIDindex args = *reinterpret_cast<pIDindex*>(entry->GetArg());
        int player = args.playerID;
        int i = args.index;

        std::string longName = entry->Name();
        std::string prefix = longName.substr(0, 5);
        if (prefix == "(  ) ")
        {
            std::string baseName = longName.substr(5, std::string::npos);
            indCostumeEffects[player].set(i);
            entry->SetName("(X) " + baseName);
        }
        else
        {
            std::string baseName = longName.substr(4, std::string::npos);
            indCostumeEffects[player].reset(i);
            entry->SetName("(  ) " + baseName);
        }
    }

    // Enables one of three dodge chances (25%, 50% or 75%)
    void Costume::luckyDodge(MenuEntry* entry)
    {
        int player = reinterpret_cast<int>(entry->GetArg());
        if (entry->Name() == "(  ) Lucky Dodge")
        {
            // Select between 3 dodge chances
            Keyboard dodge("Select the dodge rate:");
            StringVector dodgeChances =
            {
                "25%",
                "50%",
                "75%"
            };

            dodge.Populate(dodgeChances);

            switch (dodge.Open()) {
                case 0:
                    indCostumeEffects[player].set(6);
                    entry->SetName("(X) Lucky Dodge - 25%");
                    break;
                case 1:
                    indCostumeEffects[player].set(7);
                    entry->SetName("(X) Lucky Dodge - 50%");
                    break;
                case 2:
                    indCostumeEffects[player].set(41);
                    entry->SetName("(X) Lucky Dodge - 75%");
                    break;
            }
        }
        else
        {
            indCostumeEffects[player].reset(6);
            indCostumeEffects[player].reset(7);
            indCostumeEffects[player].reset(41);
            entry->SetName("(  ) Lucky Dodge");
        }
    }

    // Helper to write costume effects to status bits with bit masking to prevent overwrites of existing set bits
    void Costume::writeIndCostumeEffects(MenuEntry* entry)
    {
        int player = reinterpret_cast<int>(entry->GetArg());
        u32 attributesA;
        u8 attributesB;
        u32 addrA = AddressList::getAddress("StatusBitA") + player * PLAYER_OFFSET;
        u32 addrB = AddressList::getAddress("StatusBitE") + player * PLAYER_OFFSET;

        if (Process::Read32(addrA, attributesA) && Process::Read8(addrB, attributesB))
        {
            std::bitset<42> fullset = indCostumeEffects[player]; // contains bits for 9510
            std::bitset<42> subset = fullset >> 32;              // contains bits for 9514
            std::bitset<42> bitmask("00000000001111111111111111111111111111111");

            // If statements for the attributes with distant addresses
            if (fullset.test(40))
                Process::Write8(AddressList::getAddress("HeartDropRate"), 0x1);
            if (fullset.test(41))
                Process::Write8(AddressList::getAddress("BearDodge"), 0x1);

            fullset &= bitmask; // zero out everything to the left of fullset, leaving 32 bits of data
            bitmask >> 23;      // add more zeroes to the left of bitmask
            subset &= bitmask;  // zero out everything to the left of subset, leaving 8 bits of data

            u32 attrToWriteA = attributesA | (u32) fullset.to_ulong();
            u8 attrToWriteB = attributesB | (u8) subset.to_ulong();

            Process::Write32(addrA, attrToWriteA);
            Process::Write8(addrB, attrToWriteB);
        }
    }

    /* --------------------------------- */

    // All-Player Costume Effects

    // Kokiri Clothes Bow upgrade - Triple arrows
    void Costume::kokiri(MenuEntry* entry)
    {
        if (entry->Name() == "(  ) Kokiri Clothes - Bow upgrade")
        {
            Process::Patch(AddressList::getAddress("KokiriA"), 0xE3811001);
            Process::Patch(AddressList::getAddress("KokiriB"), 0xE5841010);
            Process::Patch(AddressList::getAddress("KokiriC"), 0xEA000005);
            Process::Patch(AddressList::getAddress("KokiriD"), 0xE3A00002);
            Process::Patch(AddressList::getAddress("KokiriE"), 0xE1CD03BC);
            Process::Patch(AddressList::getAddress("KokiriIcon"), 0xE320F000);
            entry->SetName("(X) Kokiri Clothes - Bow upgrade");
        }
        else
        {
            Process::Patch(AddressList::getAddress("KokiriA"), 0x03811001);
            Process::Patch(AddressList::getAddress("KokiriB"), 0x05841010);
            Process::Patch(AddressList::getAddress("KokiriC"), 0x0A000005);
            Process::Patch(AddressList::getAddress("KokiriD"), 0x03A00002);
            Process::Patch(AddressList::getAddress("KokiriE"), 0x01CD03BC);
            Process::Patch(AddressList::getAddress("KokiriIcon"), 0x1AFFFFFB);
            entry->SetName("(  ) Kokiri Clothes - Bow upgrade");
        }
    }

    // Big Bombs - Self-explanatory
    void Costume::bigBomb(MenuEntry* entry)
    {
        if (entry->Name() == "(  ) Big Bomb Outfit - Bomb upgrade")
        {
            Process::Patch(AddressList::getAddress("BigBombA"), 0xE5C454FE);
            Process::Patch(AddressList::getAddress("BigBombB"), 0xE1CD63BC);
            Process::Patch(AddressList::getAddress("BigBombC"), 0xEA000000);
            Process::Patch(AddressList::getAddress("BigBombIcon"), 0xE320F000);
            entry->SetName("(X) Big Bomb Outfit - Bomb upgrade");
        }
        else
        {
            Process::Patch(AddressList::getAddress("BigBombA"), 0x05C454FE);
            Process::Patch(AddressList::getAddress("BigBombB"), 0x01CD63BC);
            Process::Patch(AddressList::getAddress("BigBombC"), 0x0A000000);
            Process::Patch(AddressList::getAddress("BigBombIcon"), 0x1A000001);
            entry->SetName("(  ) Big Bomb Outfit - Bomb upgrade");
        }
    }

    // Torrent Robe Water Rod upgrade - Larger water pillars
    void Costume::torrent(MenuEntry* entry)
    {
        if (entry->Name() == "(  ) Torrent Robe - Water Rod upgrade")
        {
            Process::Patch(AddressList::getAddress("TorrentA"), 0xE3811301);
            Process::Patch(AddressList::getAddress("TorrentB"), 0xE5841010);
            Process::Patch(AddressList::getAddress("TorrentC"), 0xEA000003);
            Process::Patch(AddressList::getAddress("TorrentD"), 0xE3A00001);
            Process::Patch(AddressList::getAddress("TorrentE"), 0xEA000000);
            Process::Patch(AddressList::getAddress("TorrentIcon"), 0xE320F000);
            entry->SetName("(X) Torrent Robe - Water Rod upgrade");
        }
        else
        {
            Process::Patch(AddressList::getAddress("TorrentA"), 0x03811301);
            Process::Patch(AddressList::getAddress("TorrentB"), 0x05841010);
            Process::Patch(AddressList::getAddress("TorrentC"), 0x0A000003);
            Process::Patch(AddressList::getAddress("TorrentD"), 0x03A00001);
            Process::Patch(AddressList::getAddress("TorrentE"), 0x0A000000);
            Process::Patch(AddressList::getAddress("TorrentIcon"), 0x1AFFFFF2);
            entry->SetName("(  ) Torrent Robe - Water Rod upgrade");
        }
    }

    // Boomeranger Boomerang upgrade - Bigger, 2x player capacity and does damage
    void Costume::boomeranger(MenuEntry* entry)
    {
        if (entry->Name() == "(  ) Boomeranger - Boomerang upgrade")
        {
            Process::Patch(AddressList::getAddress("BoomerangerA"), 0xE3811602);
            Process::Patch(AddressList::getAddress("BoomerangerB"), 0xE5841014);
            Process::Patch(AddressList::getAddress("BoomerangerC"), 0xEA000003);
            Process::Patch(AddressList::getAddress("BoomerangerD"), 0xE3A00001);
            Process::Patch(AddressList::getAddress("BoomerangerE"), 0xEA000000);
            Process::Patch(AddressList::getAddress("BoomerangerF"), 0xEA000007);
            Process::Patch(AddressList::getAddress("BoomerangerIcon"), 0xE320F000);
            entry->SetName("(X) Boomeranger - Boomerang upgrade");
        }
        else
        {
            Process::Patch(AddressList::getAddress("BoomerangerA"), 0x03811602);
            Process::Patch(AddressList::getAddress("BoomerangerB"), 0x05841014);
            Process::Patch(AddressList::getAddress("BoomerangerC"), 0x0A000003);
            Process::Patch(AddressList::getAddress("BoomerangerD"), 0x03A00001);
            Process::Patch(AddressList::getAddress("BoomerangerE"), 0x0A000000);
            Process::Patch(AddressList::getAddress("BoomerangerF"), 0x0A000007);
            Process::Patch(AddressList::getAddress("BoomerangerIcon"), 0x1AFFFFF5);
            entry->SetName("(  ) Boomeranger - Boomerang upgrade");
        }
    }

    // Fire Blazer Fire Gloves upgrade - Triple fireballs and double damage
    void Costume::fireBlazer(MenuEntry* entry)
    {
        if (entry->Name() == "(  ) Fire Blazer - Fire Gloves upgrade")
        {
            Process::Patch(AddressList::getAddress("FireBlazerA"), 0xE3811008);
            Process::Patch(AddressList::getAddress("FireBlazerB"), 0xE5841014);
            Process::Patch(AddressList::getAddress("FireBlazerC"), 0xEA000009);
            Process::Patch(AddressList::getAddress("FireBlazerD"), 0xEA000007);
            Process::Patch(AddressList::getAddress("FireBlazerIcon"), 0xE320F000);
            entry->SetName("(X) Fire Blazer - Fire Gloves upgrade");
        }
        else
        {
            Process::Patch(AddressList::getAddress("FireBlazerA"), 0x03811008);
            Process::Patch(AddressList::getAddress("FireBlazerB"), 0x05841014);
            Process::Patch(AddressList::getAddress("FireBlazerC"), 0x0A000009);
            Process::Patch(AddressList::getAddress("FireBlazerD"), 0x0A000007);
            Process::Patch(AddressList::getAddress("FireBlazerIcon"), 0x1AFFFFF8);
            entry->SetName("(  ) Fire Blazer - Fire Gloves upgrade");
        }
    }

    // Gust Garb Gust Jar upgrade - Bigger, faster gusts, spins fans longer
    void Costume::gustGarb(MenuEntry* entry)
    {
        if (entry->Name() == "(  ) Gust Garb - Gust Jar upgrade")
        {
            Process::Patch(AddressList::getAddress("GustGarbA"), 0xE3811601);
            Process::Patch(AddressList::getAddress("GustGarbB"), 0xE5841010);
            Process::Patch(AddressList::getAddress("GustGarbC"), 0xEA000005);
            Process::Patch(AddressList::getAddress("GustGarbD"), 0xE28F0034);
            Process::Patch(AddressList::getAddress("GustGarbE"), 0xE320F000);
            Process::Patch(AddressList::getAddress("GustGarbF"), 0xE3A00001);
            Process::Patch(AddressList::getAddress("GustGarbG"), 0xEA000000);
            Process::Patch(AddressList::getAddress("GustGarbH"), 0xE320F000);
            Process::Patch(AddressList::getAddress("GustGarbIcon"), 0xE320F000);
            entry->SetName("(X) Gust Garb - Gust Jar upgrade");
        }
        else
        {
            Process::Patch(AddressList::getAddress("GustGarbA"), 0x03811601);
            Process::Patch(AddressList::getAddress("GustGarbB"), 0x05841010);
            Process::Patch(AddressList::getAddress("GustGarbC"), 0x0A000005);
            Process::Patch(AddressList::getAddress("GustGarbD"), 0x028F0034);
            Process::Patch(AddressList::getAddress("GustGarbE"), 0x128F0044);
            Process::Patch(AddressList::getAddress("GustGarbF"), 0x03A00001);
            Process::Patch(AddressList::getAddress("GustGarbG"), 0x0A000000);
            Process::Patch(AddressList::getAddress("GustGarbH"), 0x1A000005);
            Process::Patch(AddressList::getAddress("GustGarbIcon"), 0x1AFFFFEF);
            entry->SetName("(  ) Gust Garb - Gust Jar upgrade");
        }
    }

    // Robowear Gripshot upgrade - Bigger, faster, deals damage
    void Costume::robowear(MenuEntry* entry)
    {
        if (entry->Name() == "(  ) Robowear - Gripshot upgrade")
        {
            Process::Patch(AddressList::getAddress("RobowearA"), 0xEA000007);
            Process::Patch(AddressList::getAddress("RobowearB"), 0xE3A00001);
            Process::Patch(AddressList::getAddress("RobowearC"), 0xEA000000);
            Process::Patch(AddressList::getAddress("RobowearIcon"), 0xE320F000);
            entry->SetName("(X) Robowear - Gripshot upgrade");
        }
        else
        {
            Process::Patch(AddressList::getAddress("RobowearA"), 0x0A000007);
            Process::Patch(AddressList::getAddress("RobowearB"), 0x03A00001);
            Process::Patch(AddressList::getAddress("RobowearC"), 0x0A000000);
            Process::Patch(AddressList::getAddress("RobowearIcon"), 0x1AFFFFEC);
            entry->SetName("(  ) Robowear - Gripshot upgrade");
        }
    }

    // Hammerwear Hammer upgrade - Bigger, faster, double damage
    void Costume::hammerwear(MenuEntry* entry)
    {
        if (entry->Name() == "(  ) Hammerwear - Hammer upgrade")
        {
            Process::Patch(AddressList::getAddress("HammerwearA"), 0xE3811501);
            Process::Patch(AddressList::getAddress("HammerwearB"), 0xE5841010);
            Process::Patch(AddressList::getAddress("HammerwearC"), 0xEA000005);
            Process::Patch(AddressList::getAddress("HammerwearD"), 0xED848A1C);
            Process::Patch(AddressList::getAddress("HammerwearE"), 0xEA000004);
            Process::Patch(AddressList::getAddress("HammerwearF"), 0xE3A00001);
            Process::Patch(AddressList::getAddress("HammerwearG"), 0xEA000000);
            Process::Patch(AddressList::getAddress("HammerwearIcon"), 0xE320F000);
            entry->SetName("(X) Hammerwear - Hammer upgrade");
        }
        else
        {
            Process::Patch(AddressList::getAddress("HammerwearA"), 0x03811501);
            Process::Patch(AddressList::getAddress("HammerwearB"), 0x05841010);
            Process::Patch(AddressList::getAddress("HammerwearC"), 0x0A000005);
            Process::Patch(AddressList::getAddress("HammerwearD"), 0x0D848A1C);
            Process::Patch(AddressList::getAddress("HammerwearE"), 0x0A000004);
            Process::Patch(AddressList::getAddress("HammerwearF"), 0x03A00001);
            Process::Patch(AddressList::getAddress("HammerwearG"), 0x0A000000);
            Process::Patch(AddressList::getAddress("HammerwearIcon"), 0x1AFFFFE9);
            entry->SetName("(  ) Hammerwear - Hammer upgrade");
        }
    }

    // Goron Garb effects - Swimming in lava, touching flaming rupees, walking through fire curtains,
    // not catching fire from enemies/environment
    void Costume::goron(MenuEntry* entry)
    {
        if (entry->Name() == "(  ) Goron Garb - Full fire/lava immunity")
        {
            Process::Patch(AddressList::getAddress("GoronA"), 0xEA00000B);
            Process::Patch(AddressList::getAddress("GoronB"), 0xE320F000);
            Process::Patch(AddressList::getAddress("GoronC"), 0xE320F000);
            Process::Patch(AddressList::getAddress("GoronD"), 0xE3A00000);
            Process::Patch(AddressList::getAddress("GoronE"), 0xEA000000);
            Process::Patch(AddressList::getAddress("GoronF"), 0xE3811004);
            Process::Patch(AddressList::getAddress("GoronG"), 0xE5841010);
            Process::Patch(AddressList::getAddress("GoronH"), 0xEA000005);
            Process::Patch(AddressList::getAddress("GoronI"), 0xE3A00000);
            Process::Patch(AddressList::getAddress("GoronJ"), 0xEA000000);
            entry->SetName("(X) Goron Garb - Full fire/lava immunity");
        }
        else
        {
            Process::Patch(AddressList::getAddress("GoronA"), 0x0A00000B);
            Process::Patch(AddressList::getAddress("GoronB"), 0x13A00001);
            Process::Patch(AddressList::getAddress("GoronC"), 0x1A000000);
            Process::Patch(AddressList::getAddress("GoronD"), 0x03A00000);
            Process::Patch(AddressList::getAddress("GoronE"), 0x0A000000);
            Process::Patch(AddressList::getAddress("GoronF"), 0x03811004);
            Process::Patch(AddressList::getAddress("GoronG"), 0x05841010);
            Process::Patch(AddressList::getAddress("GoronH"), 0x0A000005);
            Process::Patch(AddressList::getAddress("GoronI"), 0x03A00000);
            Process::Patch(AddressList::getAddress("GoronJ"), 0x0A000000);
            entry->SetName("(  ) Goron Garb - Full fire/lava immunity");
        }
    }

    // Light Armor effects - Bright glow in the dark and burning foes on contact in the Coliseum
    void Costume::lightArmor(MenuEntry* entry)
    {
        if (entry->Name() == "(  ) Light Armor - Glow & burn foes")
        {
            Process::Patch(AddressList::getAddress("LightArmorA"), 0xE3A00001);
            Process::Patch(AddressList::getAddress("LightArmorB"), 0xEA000000);
            Process::Patch(AddressList::getAddress("LightArmorC"), 0xE3810A01);
            Process::Patch(AddressList::getAddress("LightArmorD"), 0xE5840014);
            Process::Patch(AddressList::getAddress("LightArmorE"), 0xE320F000);
            Process::Patch(AddressList::getAddress("LightArmorF"), 0xEA000006);
            entry->SetName("(X) Light Armor - Glow & burn foes");
        }
        else
        {
            Process::Patch(AddressList::getAddress("LightArmorA"), 0x03A00001);
            Process::Patch(AddressList::getAddress("LightArmorB"), 0x0A000000);
            Process::Patch(AddressList::getAddress("LightArmorC"), 0x03810A01);
            Process::Patch(AddressList::getAddress("LightArmorD"), 0x05840014);
            Process::Patch(AddressList::getAddress("LightArmorE"), 0x11A00001);
            Process::Patch(AddressList::getAddress("LightArmorF"), 0x0A000006);
            entry->SetName("(  ) Light Armor - Glow & burn foes");
        }
    }

    // Linebeck's Uniform - Time bonus effects only (Chest visibility covered in Rendering folder)
    void Costume::linebeck(MenuEntry* entry)
    {
        if (entry->Name() == "(  ) Linebeck's Uniform - Time bonuses")
        {
            Process::Patch(AddressList::getAddress("LinebeckTimeA"), 0xE286600A);
            Process::Patch(AddressList::getAddress("LinebeckTimeB"), 0xEA00000F);
            Process::Patch(AddressList::getAddress("LinebeckTimeC"), 0xE3A04001);
            entry->SetName("(X) Linebeck's Uniform - Time bonuses");
        }
        else
        {
            Process::Patch(AddressList::getAddress("LinebeckTimeA"), 0x0286600A);
            Process::Patch(AddressList::getAddress("LinebeckTimeB"), 0x0A00000F);
            Process::Patch(AddressList::getAddress("LinebeckTimeC"), 0x03A04001);
            entry->SetName("(  ) Linebeck's Uniform - Time bonuses");
        }
    }

    // Cheetah Costume effects - Faster walk speed
    // Affected by the "Cheetah Costume - Set speed mult." code
    void Costume::cheetah(MenuEntry* entry)
    {
        if (entry->Name() == "(  ) Cheetah Costume - Faster walk speed")
        {
            Process::Patch(AddressList::getAddress("CheetahA"), 0xED9F0A12);
            Process::Patch(AddressList::getAddress("CheetahB"), 0xED840A41);
            Process::Patch(AddressList::getAddress("CheetahC"), 0xEA000000);
            entry->SetName("(X) Cheetah Costume - Faster walk speed");
        }
        else
        {
            Process::Patch(AddressList::getAddress("CheetahA"), 0x0D9F0A12);
            Process::Patch(AddressList::getAddress("CheetahB"), 0x0D840A41);
            Process::Patch(AddressList::getAddress("CheetahC"), 0x0A000000);
            entry->SetName("(  ) Cheetah Costume - Faster walk speed");
        }
    }

    // Sword Suit / Sword Master Suit / Fierce Deity Armor - Double sword damage
    // Affected by the "Set boosted sword & hammer dmg mult." code
    void Costume::swordDmgBoost(MenuEntry* entry)
    {
        if (entry->Name() == "(  ) Sword Suits - Sword damage boost")
        {
            Process::Patch(AddressList::getAddress("SwordDamageBoostA"), 0xED848A1B);
            Process::Patch(AddressList::getAddress("SwordDamageBoostB"), 0xEA000000);
            entry->SetName("(X) Sword Suits - Sword damage boost");
        }
        else
        {
            Process::Patch(AddressList::getAddress("SwordDamageBoostA"), 0x0D848A1B);
            Process::Patch(AddressList::getAddress("SwordDamageBoostB"), 0x0A000000);
            entry->SetName("(  ) Sword Suits - Sword damage boost");
        }
    }

    // Allows you to force either singular or FDA-like quad sword beams
    void Costume::swordBeam(MenuEntry* entry)
    {
        if (entry->Name() == "(  ) Sword Suits - Set sword beam")
        {
            Keyboard beamTypes("Select a beam type:");
            StringVector beamOptions =
            {
                "Single reg. sword beam",
                "Quad reg. sword beams",
                "Single big sword beam",
                "Quad big sword beams"
            };
            beamTypes.Populate(beamOptions);
            int result = beamTypes.Open();
            if (result >= 0)
            {
                Process::Patch(AddressList::getAddress("SwordBeamAbility"), 0xE320F000);

                // Set entry name based on choice
                switch (result)
                {
                    case 0:
                        entry->SetName("(X) Sword Suits - Single reg. sword beam");
                        break;
                    case 1:
                        entry->SetName("(X) Sword Suits - Quad reg. sword beams");
                        break;
                    case 2:
                        entry->SetName("(X) Sword Suits - Single big sword beam");
                        break;
                    case 3:
                        entry->SetName("(X) Sword Suits - Quad big sword beams");
                        break;
                }

                // Patch big effect and quad effect if applicable

                if (result >= 2)
                    Process::Patch(AddressList::getAddress("BigSwordBeam"), 0xEA00001F);
                
                if (result % 2 == 1)
                {
                    Process::Patch(AddressList::getAddress("QuadBeamA"), 0xE3A05000);
                    Process::Patch(AddressList::getAddress("QuadBeamB"), 0xE1A06005);
                    Process::Patch(AddressList::getAddress("QuadBeamC"), 0xEA00000D);
                    Process::Patch(AddressList::getAddress("QuadBeamSpinA"), 0xE3A05000);
                    Process::Patch(AddressList::getAddress("QuadBeamSpinB"), 0xE1A06005);
                    Process::Patch(AddressList::getAddress("QuadBeamSpinC"), 0xEA000016);
                }
            }
        }
        else
        {
            Process::Patch(AddressList::getAddress("SwordBeamAbility"), 0x1A00000B);
            Process::Patch(AddressList::getAddress("BigSwordBeam"), 0x0A00001F);
            Process::Patch(AddressList::getAddress("QuadBeamA"), 0x03A05000);
            Process::Patch(AddressList::getAddress("QuadBeamB"), 0x01A06005);
            Process::Patch(AddressList::getAddress("QuadBeamC"), 0x0A00000D);
            Process::Patch(AddressList::getAddress("QuadBeamSpinA"), 0x03A05000);
            Process::Patch(AddressList::getAddress("QuadBeamSpinB"), 0x01A06005);
            Process::Patch(AddressList::getAddress("QuadBeamSpinC"), 0x0A000016);
            entry->SetName("(  ) Sword Suits - Set sword beam");
        }
    }

    // Fierce Deity Armor's immunity to knockback from taking damage, hitting certain enemies, etc.
    void Costume::knockbackImmunity(MenuEntry* entry)
    {
        if (entry->Name() == "(  ) Fierce Deity Armor - Knockback immunity")
        {
            Process::Patch(AddressList::getAddress("KnockbackImmunity"), 0xEA00002C);
            entry->SetName("(X) Fierce Deity Armor - Knockback immunity");
        }
        else
        {
            Process::Patch(AddressList::getAddress("KnockbackImmunity"), 0x0A00002C);
            entry->SetName("(  ) Fierce Deity Armor - Knockback immunity");
        }
    }

    /* --------------------------------- */

    // Bonus Effects

    // Set number of Tingle Balloons
    void Costume::tingle(MenuEntry* entry)
    {
        int linkChoice = GeneralHelpers::chooseLink();

        if (linkChoice >= 0)
        {
            std::string selectedPlayer = GeneralHelpers::getPlayerAsStr(linkChoice);
            std::string currentBalloons;
            if (infTingleBalloons[linkChoice])
            {
                currentBalloons = "Infinite";
            }
            else
            {
                u8 balloons;
                Process::Read8(AddressList::getAddress("TingleBalloons") + linkChoice*PLAYER_OFFSET, balloons);
                currentBalloons = std::to_string(balloons);
            }

            std::string topscreenMessage = "Enter a positive number of balloons:\n\n";
            topscreenMessage += "Enter 256 or higher for infinite balloons.\nEnter 0 to reset for this player.\nYou must reset to disable infinite balloons.\n\n";
            topscreenMessage += "Selected: " + selectedPlayer + " - " + currentBalloons;
            Keyboard balloons(topscreenMessage);
            balloons.IsHexadecimal(false);
            float result;
            balloons.Open(result);
            if ((int)result >= 256)
            {
                infTingleBalloons[linkChoice] = true;
                infBalloonsAuto->Enable();
            }
            else if ((int)result == 0)
            {
                infTingleBalloons[linkChoice] = false;
                Process::Write8(AddressList::getAddress("TingleBalloons") + linkChoice*PLAYER_OFFSET, 0);
                if (!infTingleBalloons[0] && !infTingleBalloons[1] && !infTingleBalloons[2])
                    infBalloonsAuto->Disable();
            }
            else if ((int)result > 0)
            {
                Process::Write8(AddressList::getAddress("TingleBalloons") + linkChoice*PLAYER_OFFSET, result);
            }
        }
    }

    // Infinite balloons
    void Costume::writeBalloons(MenuEntry* entry)
    {
        for (int iterateThruPlayers = 0; iterateThruPlayers < 3; iterateThruPlayers++)
        {
            if (infTingleBalloons[iterateThruPlayers])
            {
                // Constantly overwrite 1 balloon
                // When disabled, only one extra balloon to pop before back to normal
                Process::Write8(AddressList::getAddress("TingleBalloons") + iterateThruPlayers*PLAYER_OFFSET, 1);
            }
        }
    }

    // Choose whether Dapper spin attacks happen in one, two or three swings
    void Costume::dapperInstant(MenuEntry* entry)
    {
        int linkChoice = GeneralHelpers::chooseLink();

        if (linkChoice >= 0)
        {
            std::string selectedPlayer = GeneralHelpers::getPlayerAsStr(linkChoice);
            std::string currentDapperStatus;
            if (dapperSpinA[linkChoice])
            {
                if (dapperSpinB[linkChoice])
                    currentDapperStatus = "One swing";
                else
                    currentDapperStatus = "Two swings";
            }
            else
            {
                currentDapperStatus = "Not edited";
            }

            std::string topscreenMessage = "Spin attack in one swing, two swings, or reset to\nthree swings?\n\n";
            topscreenMessage += "Selected: " + selectedPlayer + " - " + currentDapperStatus;
            Keyboard dapperSwing(topscreenMessage);
            StringVector dapperOptions =
            {
                "One swing",
                "Two swings",
                "Reset"
            };
            dapperSwing.Populate(dapperOptions);

            switch(dapperSwing.Open())
            {
                case 0:
                    dapperSpinA[linkChoice] = true;
                    dapperSpinB[linkChoice] = true;
                    dapperAuto->Enable();
                    break;
                case 1:
                    dapperSpinA[linkChoice] = true;
                    dapperSpinB[linkChoice] = false;
                    dapperAuto->Enable();
                    break;
                case 2:
                    dapperSpinA[linkChoice] = false;
                    dapperSpinB[linkChoice] = false;
                    if (!dapperSpinA[0] && !dapperSpinA[1] && !dapperSpinA[2] && !dapperSpinB[0] && !dapperSpinB[1] && !dapperSpinB[2])
                        dapperAuto->Disable();
                    break;
            }
        }
    }

    // Driver code for the dapper reduced swings to spin effect
    void Costume::writeDapper(MenuEntry* entry)
    {
        for (int iterateThruPlayers = 0; iterateThruPlayers < 3; iterateThruPlayers++)
        {
            // Initialize "Two swings in a row"; player needs to swing once to reset the countdown and then a second time within 30ms to spin attack
            if (dapperSpinA[iterateThruPlayers])
                Process::Write8(AddressList::getAddress("DapperSpinCheck") + iterateThruPlayers*PLAYER_OFFSET, 1);
            // Freeze countdown at 0x1E (30ms) so you have infinite time for your next swing to count
            // When combined with the above, your first swing is always a spin attack
            if (dapperSpinB[iterateThruPlayers])
                Process::Write8(AddressList::getAddress("DapperSwingCountdown") + iterateThruPlayers*PLAYER_OFFSET, 0x1E);
        }
    }

    // Menu interface for force-toggling beam cooldowns for individual players
    void Costume::selectLinkBeam(MenuEntry *entry)
    {
        std::string title;
        StringVector bottomScreenOptions;
        Keyboard kbd("Menu");

        kbd.CanAbort(false);
        bool loop = true;

        while (loop)
        {
            title = "Use the toggles to disable the Sword Beam cooldown period:\n\n";

            bottomScreenOptions.clear();
            bottomScreenOptions.push_back(std::string("Player 1 ") << (beamStatuses[0] ? ENABLED_SLIDER : DISABLED_SLIDER));
            bottomScreenOptions.push_back(std::string("Player 2 ") << (beamStatuses[1] ? ENABLED_SLIDER : DISABLED_SLIDER));
            bottomScreenOptions.push_back(std::string("Player 3 ") << (beamStatuses[2] ? ENABLED_SLIDER : DISABLED_SLIDER));
            bottomScreenOptions.push_back("Save changes");
            bottomScreenOptions.push_back("Disable entry");

            kbd.GetMessage() = title;
            kbd.Populate(bottomScreenOptions);

            switch (kbd.Open())
            {
                case 0:
                    beamStatuses[0] = !beamStatuses[0];
                    break;
                case 1:
                    beamStatuses[1] = !beamStatuses[1];
                    break;
                case 2:
                    beamStatuses[2] = !beamStatuses[2];
                    break;
                case 3:
                    beamCooldownAuto->Enable();
                    loop = false;
                    break;
                default:
                    beamCooldownAuto->Disable();
                    loop = false;
                    break;
            }
        }
    }

    // Helper function to write cooldown edits to memory
    void Costume::writeBeamCooldown(MenuEntry *entry)
    {
        u8 minBeamCooldownTimer = 0x1E;

        for (int iterateThruPlayers = 0; iterateThruPlayers < 3; iterateThruPlayers++)
        {
            if (beamStatuses[iterateThruPlayers])
                Process::Write8(AddressList::getAddress("SwordBeamCD") + (PLAYER_OFFSET * iterateThruPlayers), minBeamCooldownTimer);
        }
    }

    // Sets the speed multiplier for the Cheetah Costume (default 1.5)
    void Costume::setCheetahMultiplier(MenuEntry* entry)
    {
        float newSpeed = 1;
        Keyboard editSpeed("Enter a custom speed multiplier.\n\nThe default value is 1.5.\nNegative values will invert movement.");
        editSpeed.IsHexadecimal(false);
        if (editSpeed.Open(newSpeed) == 0)
        {
            entry->SetName("Cheetah Costume - Set speed mult.: "+std::to_string(newSpeed)+"x");
            Process::WriteFloat(AddressList::getAddress("SpeedMultiplierCheetah"), newSpeed);
        }
    }

    // Sets the damage dealt by the Cacto Clothes to enemies in the Drablands (default 2 hearts)
    void Costume::setCactoDmg(MenuEntry* entry)
    {
        float newDmg = 1;
        Keyboard editDmg("Enter a custom damage amount as a positive integer\nrepresenting the number of hearts of damage.\n\nThe default value is 2. Minimum is 1.");
        editDmg.IsHexadecimal(false);
        if (editDmg.Open(newDmg) == 0)
        {
            // Minimum 1 heart damage
            if (newDmg < 1)
                newDmg = 1;

            // Integer value as # hearts to write in entry name
            u8 dmgToWrite = (u8)newDmg;
            std::string unit = "";
            if (dmgToWrite == 1)
                unit = " heart";
            else
                unit = " hearts";
            
            entry->SetName("Cacto Clothes - Set damage: "+std::to_string(dmgToWrite)+unit);
            // Write to address in terms of quarter hearts
            Process::Write8(AddressList::getAddress("CactoDamageDrablands"), dmgToWrite*4);
        }
    }

    // Sets the damage multiplier used to boost the sword or hammer damage when an appropriate costume is worn (default 2.0)
    void Costume::setSwordHammerDmgMult(MenuEntry* entry)
    {
        float newDmg = 1;
        Keyboard editDmg("Enter a custom damage multiplier.\n\nThe default value is 2.\nNegative values equate to 0 damage.");
        editDmg.IsHexadecimal(false);
        if (editDmg.Open(newDmg) == 0)
        {
            entry->SetName("Set boosted sword & hammer dmg mult.: "+std::to_string(newDmg)+"x");
            Process::WriteFloat(AddressList::getAddress("SwordHammerDmgBoost"), newDmg);
        }
    }

    // Enables spawning sword beams at any health value, instead of only at full health
    void Costume::swordBeamsAnyHealth(MenuEntry* entry)
    {
        if (entry->Name() == "Enable sword beams at any health")
        {
            Process::Patch(AddressList::getAddress("SwordBeamHealthCheckA"), 0xE3A00001);
            Process::Patch(AddressList::getAddress("SwordBeamHealthCheckB"), 0xEA000000);
            entry->SetName("Reset to sword beams only at max health");
        }
        else
        {
            Process::Patch(AddressList::getAddress("SwordBeamHealthCheckA"), 0x03A00001);
            Process::Patch(AddressList::getAddress("SwordBeamHealthCheckB"), 0x0A000000);
            entry->SetName("Enable sword beams at any health");
        }
    }
}