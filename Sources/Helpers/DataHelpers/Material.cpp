#include "Helpers.hpp"
#include "AddressList.hpp"

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
        "Mystery Jade",
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
        "Beastie Patch"
    };

    const StringVector Material::fortressMats = {
        "Stiff Puff",
        "Silver Thread",
        "Royal Ring",
        "Ancient Coin",
        "Fancy Fabric",
        "Exquisite Lace",
        "Vibrant Brooch",
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

    int Material::selectMaterialWorld(void)
    {
        StringVector worldSelectionList = Level::worldList;

        worldSelectionList.insert(worldSelectionList.begin(), "Other");

        Keyboard chooseWorld("Select a world:");
        chooseWorld.Populate(worldSelectionList);

        return chooseWorld.Open();
    }

    int Material::selectMaterialIndiv(int selectedWorld)
    {
        int materialOffset = 0, chosenMaterial = -1;
        Keyboard chooseMaterial("Select a material:");

        switch (selectedWorld) {
        case 0:
            chooseMaterial.Populate(otherMats);
            materialOffset = 0;
            break;

        case 1:
            chooseMaterial.Populate(woodlandMats);
            materialOffset = 6;
            break;

        case 2:
            chooseMaterial.Populate(riversideMats);
            materialOffset = 14;
            break;

        case 3:
            chooseMaterial.Populate(volcanoMats);
            materialOffset = 22;
            break;

        case 4:
            chooseMaterial.Populate(iceMats);
            materialOffset = 30;
            break;

        case 5:
            chooseMaterial.Populate(fortressMats);
            materialOffset = 38;
            break;

        case 6:
            chooseMaterial.Populate(dunesMats);
            materialOffset = 46;
            break;

        case 7:
            chooseMaterial.Populate(ruinsMats);
            materialOffset = 54;
            break;

        case 8:
            chooseMaterial.Populate(skyMats);
            materialOffset = 62;
            break;
        }
        chosenMaterial = chooseMaterial.Open();
        if (chosenMaterial >= 0)
            chosenMaterial += (materialOffset + 20); // 0-23 are not materials

        return chosenMaterial;
    }

    std::string Material::getMaterialName(int world, int mat)
    {
        int finalMatID = mat - 20;

        switch (world) {
        case 0:
            return Material::otherMats[finalMatID];
        case 1:
            return Material::woodlandMats[finalMatID - 6];
        case 2:
            return Material::riversideMats[finalMatID - 14];
        case 3:
            return Material::volcanoMats[finalMatID - 22];
        case 4:
            return Material::iceMats[finalMatID - 30];
        case 5:
            return Material::fortressMats[finalMatID - 38];
        case 6:
            return Material::dunesMats[finalMatID - 46];
        case 7:
            return Material::ruinsMats[finalMatID - 54];
        case 8:
            return Material::skyMats[finalMatID - 62];
        }
        return "";
    }

    // TODO: review this
    int Material::getMaterialCount(int materialID)
    {
        // is this needed
        return 0;
    }
}