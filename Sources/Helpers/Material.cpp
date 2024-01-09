#include "cheats.hpp"
#include "Helpers/GameData.hpp"
#include "Helpers/Material.hpp"

#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework {
    const StringVector Material::otherMats = {
        "Lady's Glasses",
        "Lady's Collar",
        "Lady's Parasol",
        "Frilly Fabric",
        "Friendly Token",
        "Freebie"
    };

    const StringVector Material::woodlandMats = {
        "Blob Jelly",
        "Armos Spirit",
        "Tektite Shell",
        "Sweet Shroom",
        "Blin Bling",
        "Gohma's Eye",
        "Mystery Jade"
        "Supple Leaf"
    };

    const StringVector Material::riversideMats = {
        "Fresh Kelp",
        "Zora Scale",
        "Hytopia Silk",
        "Aqua Crown",
        "Octorok Suckers",
        "Fairy Dust",
        "Rainbow Coral",
        "Thornberry"
    };

    const StringVector Material::volcanoMats = {
        "Goron Ore",
        "Monster Guts",
        "Demon Fossil",
        "Rugged Horn",
        "Kodongo Tail",
        "Keese Wing",
        "Star Fragment",
        "Divine Whiskers"
    };

    const StringVector Material::iceMats = {
        "Fluffy Fuzz",
        "Tiny Snowflake",
        "Serpent Fangs",
        "Crimson Shell",
        "Freezard Water",
        "Chill Stone",
        "Ice Rose",
        "Beastie Patch",
    };

    const StringVector Material::fortressMats = {
        "Stiff Puff",
        "Silver Thread",
        "Royal Ring",
        "Ancient Coin",
        "Fancy Fabric",
        "Exquisite Lace",
        "Vibrant Brooch"
        "Brittle Papyrus"
    };

    const StringVector Material::dunesMats = {
        "Palm Cone",
        "Ancient Fin",
        "Vintage Linen",
        "Gibdo Bandage",
        "Stal Skull",
        "Sandy Ribbon",
        "Crystal Skull",
        "Golden Insect"
    };

    const StringVector Material::ruinsMats = {
        "Carrumpkin",
        "Mystery Extract",
        "Spider Silk Lace",
        "Poe Soul",
        "Twisted Twig",
        "Lava Drop",
        "Sanctuary Mask",
        "Gold Dust"
    };

    const StringVector Material::skyMats = {
        "Cucco Feathers",
        "Carmine Pearl",
        "Sky Dragon Tail",
        "Pretty Plume",
        "Mock Fairy",
        "Aurora Stone",
        "Steel Mask",
        "Fabled Butterfly"
    };

    int Material::selectMaterialWorld(void) {
        StringVector worldSelectionList = GameData::worldList;

        worldSelectionList.insert(worldSelectionList.begin(), "Other");

        Keyboard chooseWorld("Select a world:");
        chooseWorld.Populate(worldSelectionList);

        return chooseWorld.Open();
    }

    int Material::selectMaterialIndiv(int selectedWorld) {
        u8 chosenMaterial;
        Keyboard chooseMaterial("Select a material:");

        switch (selectedWorld) {
        case 0:
            chooseMaterial.Populate(otherMats);
            chosenMaterial = chooseMaterial.Open();
            break;

        case 1:
            chooseMaterial.Populate(woodlandMats);
            chosenMaterial = chooseMaterial.Open() + 6;
            break;

        case 2:
            chooseMaterial.Populate(riversideMats);
            chosenMaterial = chooseMaterial.Open() + 14;
            break;

        case 3:
            chooseMaterial.Populate(volcanoMats);
            chosenMaterial = chooseMaterial.Open() + 22;
            break;

        case 4:
            chooseMaterial.Populate(iceMats);
            chosenMaterial = chooseMaterial.Open() + 30;
            break;

        case 5:
            chooseMaterial.Populate(fortressMats);
            chosenMaterial = chooseMaterial.Open() + 38;
            break;

        case 6:
            chooseMaterial.Populate(dunesMats);
            chosenMaterial = chooseMaterial.Open() + 46;
            break;

        case 7:
            chooseMaterial.Populate(ruinsMats);
            chosenMaterial = chooseMaterial.Open() + 54;
            break;

        case 8:
            chooseMaterial.Populate(skyMats);
            chosenMaterial = chooseMaterial.Open() + 62;
            break;
        }

        chosenMaterial = chosenMaterial + 20; // 0-23 are not materials
        return chosenMaterial;
    }
}