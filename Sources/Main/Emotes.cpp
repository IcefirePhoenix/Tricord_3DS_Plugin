#include "Helpers.hpp"
#include "Main/Emotes.hpp"

namespace CTRPluginFramework
{
	// layout A: over here, bye, lets go, hello, cheer, frown, no, thumbs up
	// layout B: item, throw, totem, yawn, cheer, frown, nooo, blank

	u32 blankEmotePointer = 0x2070A780;

	// sorted by region... may break if custom files are loaded
	// TODO: look into possible pointers to get rid of these arrays

	// column order: NA, EU, JP, KOR
	const u32 greenEmotePointers[12][4] = {
		{ 0x20307780, 0x208E9080, 0x20330C80, 0x202E0D80 }, // over here
		{ 0x20308780, 0x208EA080, 0x20331C80, 0x202E1D80 }, // item
		{ 0x20309780, 0x208EB080, 0x20332C80, 0x202E2D80 }, // throw
		{ 0x2030A780, 0x208EC080, 0x20333C80, 0x202E3D80 }, // shrug/bow
		{ 0x2030B780, 0x208ED080, 0x20334C80, 0x202E4D80 }, // no
		{ 0x2030C780, 0x208EE080, 0x20335C80, 0x202E5D80 }, // hello
		{ 0x2030D780, 0x208EF080, 0x20336C80, 0x202E6D80 }, // lets go
		{ 0x20C86080, 0x20D1B480, 0x20C6A680, 0x20BE8080 }, // totem
		{ 0x20C87080, 0x20D1C480, 0x20C37680, 0x20BE9080 }, // cheer
		{ 0x20C88080, 0x20D1D480, 0x20C38680, 0x20BEA080 }, // thumbs up
		{ 0x20C89080, 0x20D1E480, 0x20C39680, 0x20BEB080 }, // yawn
		{ 0x20C8A080, 0x20D1F480, 0x20C3A680, 0x20BEC080 }  // bye
	};

	const u32 blueEmotePointers[12][4] = {
		{ 0x20300780, 0x208E2080, 0x20329C80, 0x202D9D80 }, // over here
		{ 0x20301780, 0x208E3080, 0x2032AC80, 0x202DAD80 }, // item
		{ 0x20302780, 0x208E4080, 0x2032BC80, 0x202DBD80 }, // throw
		{ 0x20303780, 0x208E5080, 0x2032CC80, 0x202DCD80 }, // shrug/bow
		{ 0x20304780, 0x208E6080, 0x2032DC80, 0x202DDD80 }, // no
		{ 0x20305780, 0x208E7080, 0x2032EC80, 0x202DED80 }, // hello
		{ 0x20306780, 0x208E8080, 0x2032FC80, 0x202DFD80 }, // lets go
		{ 0x20C81080, 0x20D16480, 0x20C31680, 0x20BE3080 }, // totem
		{ 0x20C82080, 0x20D17480, 0x20C32680, 0x20BE4080 }, // cheer
		{ 0x20C83080, 0x20D18480, 0x20C33680, 0x20BE5080 }, // thumbs up
		{ 0x20C84080, 0x20D19480, 0x20C34680, 0x20BE6080 }, // yawn
		{ 0x20C85080, 0x20D1A480, 0x20C35680, 0x20BE7080 } // bye
	};

	const u32 redEmotePointers[12][4] = {
		{ 0x2030E780, 0x208F0080, 0x20337C80, 0x202E7D80 }, // over here
		{ 0x2030F780, 0x208F1080, 0x20338C80, 0x202E8D80 }, // item
		{ 0x20310780, 0x208F2080, 0x20339C80, 0x202E9D80 }, // throw
		{ 0x20311780, 0x208F3080, 0x2033AC80, 0x202EAD80 }, // frown/bow
		{ 0x20312780, 0x208F4080, 0x2033BC80, 0x202EBD80 }, // no
		{ 0x20313780, 0x208F5080, 0x2033CC80, 0x202ECD80 }, // hello
		{ 0x20314780, 0x208F6080, 0x2033DC80, 0x202EDD80 }, // lets go
		{ 0x20C8B080, 0x20D20480, 0x20C3B680, 0x20BED080 }, // totem
		{ 0x20C8C080, 0x20D21480, 0x20C3C680, 0x20BEE080 }, // cheer
		{ 0x20C8D080, 0x20D22480, 0x20C3D680, 0x20BEF080 }, // thumbs up
		{ 0x20C8E080, 0x20D23480, 0x20C3E680, 0x20BF0080 }, // yawn
		{ 0x20C8F080, 0x20D24480, 0x20C3F680, 0x20BF1080 } // bye
	};

	u32 graphicsAddresses[8];

	void initEmoteAddresses(void)
	{
		Address temp[8] = {
			AddressList::EmoteAGraphic.addr,
			AddressList::EmoteBGraphic.addr,
			AddressList::EmoteCGraphic.addr,
			AddressList::EmoteDGraphic.addr,
			AddressList::EmoteEGraphic.addr,
			AddressList::EmoteFGraphic.addr,
			AddressList::EmoteGGraphic.addr,
			AddressList::EmoteHGraphic.addr
		};

		for (int iterator = 0; iterator < 8; ++iterator)
		{
			graphicsAddresses[iterator] = temp[iterator].addr;
		}
	}

	void refreshEmoteGraphics(const u32 color[][4], std::vector<u8>& layout, bool useBlank)
	{
		int region;
		switch (Process::GetTitleID())
		{
		case TID_USA:
			region = 0;
			break;

		case TID_EUR:
			region = 1;
			break;

		case TID_JPN:
			region = 2;
			break;

		case TID_KOR:
			region = 3;
			break;

		default:
			OSD::Notify("Emote Swapper: Cannot determine region.", Color::Red);
			break;
		}

		// write graphic pointers
		for (int i = 0; i < 8; i++)
		{
			if (useBlank && i == 7)
				Process::Write32(graphicsAddresses[i], blankEmotePointer);
			else
				Process::Write32(graphicsAddresses[i], color[layout[i]][region]);
		}
	}

	std::vector<u8> currentEmoteLayout;
	bool useBlankEmote;

	void initGraphicsRefresh(bool useBlankEmote)
	{
		switch (GeneralHelpers::getCurrLink())
		{
		case 0:
			refreshEmoteGraphics(greenEmotePointers, currentEmoteLayout, useBlankEmote);
			break;
		case 1:
			refreshEmoteGraphics(blueEmotePointers, currentEmoteLayout, useBlankEmote);
			break;
		case 2:
			refreshEmoteGraphics(redEmotePointers, currentEmoteLayout, useBlankEmote);
			break;
		}
	}

	void initEmoteValueLayout(u32 address, u32 buttonID_A, u32 buttonID_B, bool isBlankEmote, std::vector<u8> pointerVector)
	{
		Process::Patch(address, buttonID_A); 		// edit button IDs (1st half)
		Process::Patch(address + 0x4, buttonID_B); 	// edit button IDs (2nd half)

		currentEmoteLayout = pointerVector; 		// config pointers to emote graphics

		initGraphicsRefresh(isBlankEmote);			// update emote graphics
	}

	void toggleDefaultEmotes(bool shouldDisable)
	{
		u32 forceDotLayoutEdit = shouldDisable ? 0xEA00000D : 0x0A00000D; // edit : default
		Process::Patch(AddressList::UseDoTLayoutAlways.addr, forceDotLayoutEdit);
	}


	void Emotes::drablandEmoteSwapper(MenuEntry* entry)
	{
		if (entry->WasJustActivated())
			initEmoteAddresses();

		if (GeneralHelpers::isLoadingScreen())
		{
			initEmoteValueLayout(AddressList::GameplayEmotes.addr, 0x03020100, 0x07060504, false, {0, 1, 2, 7, 8, 3, 4, 9});
			toggleDefaultEmotes(false);
			return;
		}

		if (entry->Hotkeys[0].IsPressed())
		{
			initEmoteValueLayout(AddressList::GameplayEmotes.addr, 0x03020100, 0x07060504, false, {0, 1, 2, 7, 8, 3, 4, 9});
			toggleDefaultEmotes(true);
		}
		else if (entry->Hotkeys[1].IsPressed())
		{
			initEmoteValueLayout(AddressList::GameplayEmotes.addr, 0x09080A0B, 0x0C060504, true, {11, 6, 5, 10, 8, 3, 4});
			toggleDefaultEmotes(true);
		}
	}

	// TODO: add blank emote?
	void Emotes::lobbyEmoteSwapper(MenuEntry* entry)
	{
		u32 finalAddress = (Level::getPrevLevel() < Level::levelIDFromName("Deku Forest")) ? AddressList::GameplayEmotes.addr : AddressList::LobbyEmotes.addr;

		if (entry->WasJustActivated())
			initEmoteAddresses();

		if (GeneralHelpers::isLoadingScreen())
		{
			initEmoteValueLayout(finalAddress, 0x05080A0B, 0x00000706, true, {11, 6, 5, 3, 4, 9, 0});
			toggleDefaultEmotes(false);
			return;
		}

		if (Level::getCurrLevel() == Level::levelIDFromName("Hytopia Castle"))
		{
			// temporary workaround for when previous location is not drablands
			if (entry->Hotkeys[0].IsPressed())
			{
				initEmoteValueLayout(finalAddress, 0x05080A0B, 0x00000706, true, {11, 6, 5, 3, 4, 9, 0});
				toggleDefaultEmotes(true);
			}
			else if (entry->Hotkeys[1].IsPressed())
			{
				initEmoteValueLayout(finalAddress, 0x04090100, 0x00000302, true, {0, 1, 10, 8, 2, 7, 0});
				toggleDefaultEmotes(true);
			}
			else if (entry->Hotkeys[2].IsPressed())
			{
				Process::Write32(graphicsAddresses[5], blankEmotePointer);
				Process::Write8(finalAddress + 0x5, 0xC);
				toggleDefaultEmotes(true);
			}
		}
	}
}
