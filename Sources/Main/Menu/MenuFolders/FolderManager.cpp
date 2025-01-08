#include "Helpers/MenuHelper.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    MenuFolder *emotes = nullptr;
    MenuFolder *chaos = nullptr;
    MenuFolder *turbo = nullptr;
    MenuFolder *effects = nullptr;
    MenuFolder *gameplay = nullptr;
    MenuFolder *warp = nullptr;
    MenuFolder *healthFairies = nullptr;
    MenuFolder *energy = nullptr;
    MenuFolder *enemies = nullptr;
    MenuFolder *physics = nullptr;
    MenuFolder *moonJumpFlight = nullptr;
    MenuFolder *costume = nullptr;
    MenuFolder *miscellaneous = nullptr;
    MenuFolder *player = nullptr;
    MenuFolder *items = nullptr;
    MenuFolder *render = nullptr;
    MenuFolder *save = nullptr;
    MenuFolder *sound = nullptr;

    void CreateMenu(PluginMenu &menu)
    {
        InitFreecamCodes(menu);
        InitGameModes(menu);
        InitGameplayFolder(menu);
        InitEmoteFolder(menu);
        InitCostumeFolder(menu);
        InitMiscFolder(menu);
        InitPlayerFolder(menu);
        InitItemsFolder(menu);
        InitRenderFolder(menu);
        InitSaveFolder(menu);
        InitSoundFolder(menu);

        AddAllFolders(menu);
    }

    void AddAllFolders(PluginMenu &menu)
    {
        menu += chaos;
        menu += turbo;
        menu += costume;
        menu += emotes;
        menu += player;
        menu += gameplay;
        menu += items;
        menu += render;
        menu += save;
        menu += sound;
        menu += miscellaneous;
    }

    void ToggleMenuChange(Time time)
    {
        // freecam menu
        if (PluginMenu::GetRunningInstance()->FreecamToggle)
        {
            menuFreecam->Show();
            editFreecamControls->Show();
            editFreecamSen->Show();
            swapZoom->Show();

            chaos->HideWithoutDisable();
            turbo->HideWithoutDisable();
            costume->HideWithoutDisable();
            emotes->HideWithoutDisable();
            player->HideWithoutDisable();
            gameplay->HideWithoutDisable();
            items->HideWithoutDisable();
            render->HideWithoutDisable();
            save->HideWithoutDisable();
            sound->HideWithoutDisable();
            miscellaneous->HideWithoutDisable();
        }
        else // gameplay OR normal menu
        {
            menuFreecam->HideWithoutDisable();
            editFreecamControls->HideWithoutDisable();
            editFreecamSen->HideWithoutDisable();
            swapZoom->HideWithoutDisable();

            if (!PluginMenu::GetRunningInstance()->GameplayToggle)
            {
                chaos->Hide();
                turbo->Hide();
                costume->Show();
                emotes->Show();
                player->Show();
                gameplay->Show();
                items->Show();
                render->Show();
                save->Show();
                sound->Show();
                miscellaneous->Show();
            }
            else
            {
                menuFreecam->HideWithoutDisable();
                editFreecamControls->HideWithoutDisable();
                editFreecamSen->HideWithoutDisable();
                swapZoom->HideWithoutDisable();

                chaos->Show();
                turbo->Show();
                costume->HideWithoutDisable();
                emotes->HideWithoutDisable();
                player->HideWithoutDisable();
                gameplay->HideWithoutDisable();
                items->HideWithoutDisable();
                render->HideWithoutDisable();
                save->HideWithoutDisable();
                sound->HideWithoutDisable();
                miscellaneous->HideWithoutDisable();
            }
        }
    }
}