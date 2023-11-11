#include "cheats.hpp"
#include "AddressList.hpp"
#include "Helpers/Address.hpp"
#include "3ds.h"

namespace CTRPluginFramework
{
	u32 blankEmotePointer = 0x2070A780;

	// sorted by region... may break if custom files are loaded
	// TODO: look into possible pointers to get rid of these arrays

	// column order: NA, EU, JP, KOR
	const u32 greenEmotePointers[12][4] = {
		{ 0x20307780, 0x0, 0x0, 0x0 }, // over here
		{ 0x20308780, 0x0, 0x0, 0x0 }, // item
		{ 0x20309780, 0x0, 0x0, 0x0 }, // throw
		{ 0x2030A780, 0x0, 0x0, 0x0 }, // shrug/bow
		{ 0x2030B780, 0x0, 0x0, 0x0 }, // no
		{ 0x2030C780, 0x0, 0x0, 0x0 }, // hello
		{ 0x2030D780, 0x0, 0x0, 0x0 }, // lets go
		{ 0x20C86080, 0x0, 0x0, 0x0 }, // totem
		{ 0x20C87080, 0x0, 0x0, 0x0 }, // cheer
		{ 0x20C88080, 0x0, 0x0, 0x0 }, // thumbs up
		{ 0x20C89080, 0x0, 0x0, 0x0 }, // yawn
		{ 0x20C8A080, 0x0, 0x0, 0x0 }  // bye
	};

	const u32 blueEmotePointers[12][4] = {
		{ 0x20300780, 0x0, 0x0, 0x0 }, // over here
		{ 0x20301780, 0x0, 0x0, 0x0 }, // item
		{ 0x20302780, 0x0, 0x0, 0x0 }, // throw
		{ 0x20303780, 0x0, 0x0, 0x0 }, // shrug/bow
		{ 0x20304780, 0x0, 0x0, 0x0 }, // no
		{ 0x20305780, 0x0, 0x0, 0x0 }, // hello
		{ 0x20306780, 0x0, 0x0, 0x0 }, // lets go
		{ 0x20C81080, 0x0, 0x0, 0x0 }, // totem
		{ 0x20C82080, 0x0, 0x0, 0x0 }, // cheer
		{ 0x20C83080, 0x0, 0x0, 0x0 }, // thumbs up
		{ 0x20C84080, 0x0, 0x0, 0x0 }, // yawn
		{ 0x20C85080, 0x0, 0x0, 0x0 } // bye
	};

	const u32 redEmotePointers[12][4] = {
		{ 0x2030E780, 0x0, 0x0, 0x0 }, // over here
		{ 0x2030F780, 0x0, 0x0, 0x0 }, // item
		{ 0x20310780, 0x0, 0x0, 0x0 }, // throw
		{ 0x20311780, 0x0, 0x0, 0x0 }, // frown/bow
		{ 0x20312780, 0x0, 0x0, 0x0 }, // no
		{ 0x20313780, 0x0, 0x0, 0x0 }, // hello
		{ 0x20314780, 0x0, 0x0, 0x0 }, // lets go
		{ 0x20C8B080, 0x0, 0x0, 0x0 }, // totem
		{ 0x20C8C080, 0x0, 0x0, 0x0 }, // cheer
		{ 0x20C8D080, 0x0, 0x0, 0x0 }, // thumbs up
		{ 0x20C8E080, 0x0, 0x0, 0x0 }, // yawn
		{ 0x20C8F080, 0x0, 0x0, 0x0 } // bye
	};

	// reset
	//if (!entry->IsActivated()) {
	//	Process::Patch(AddressList::UseDoTLayoutAlways.addr, 0x0A00000D);
	//}

	void refreshEmoteGraphics(const u32 color[][4], std::vector<u8>& layout, bool useBlank) {
		u32 graphicsAddresses[8] = {
			AddressList::EmoteAGraphic.addr,
			AddressList::EmoteBGraphic.addr,
			AddressList::EmoteCGraphic.addr,
			AddressList::EmoteDGraphic.addr,
			AddressList::EmoteEGraphic.addr,
			AddressList::EmoteFGraphic.addr, 
			AddressList::EmoteGGraphic.addr,
			AddressList::EmoteHGraphic.addr
		};

		int region;
		switch (Process::GetTitleID()) {
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
		for (int i = 0; i < 8; i++) {
			if (useBlank && i == 7) {
				Process::Write32(graphicsAddresses[i], blankEmotePointer);
			}
			else {
				Process::Write32(graphicsAddresses[i], color[layout[i]][region]);
			}
		}
	}

	std::vector<u8> currentEmoteLayout;
	bool useBlankEmote;

	void initGraphicsRefresh(u8 &currentLink) {
		switch (currentLink) {
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

	// this is broken on citra...
	void drablandEmoteSwapper(MenuEntry* entry) {
		// disable default drablands emotes
		Process::Patch(AddressList::UseDoTLayoutAlways.addr, 0xEA00000D);

		// get active link color
		u8 currentLink;
		Process::Read8(AddressList::ActiveLink.addr, currentLink);

		// layout A: over here, bye, lets go, hello, cheer, frown, no, thumbs up
		if (entry->Hotkeys[0].IsPressed()) {
			Process::Patch(AddressList::GameplayEmotes.addr, 0x080A0B00); // edit button IDs (1st half)
			Process::Patch(AddressList::GameplayEmotes.addr + 0x4, 0x07060504); // edit button IDs (2nd half)
			
			currentEmoteLayout = { 0, 11, 6, 5, 8, 3, 4, 9 }; // config pointers
			useBlankEmote = false;

			initGraphicsRefresh(currentLink);
		}

		// layout B: item, throw, totem, yawn, cheer, frown, nooo, blank
		else if (entry->Hotkeys[1].IsPressed()) {
			Process::Patch(AddressList::GameplayEmotes.addr, 0x09030201);
			Process::Patch(AddressList::GameplayEmotes.addr + 0x4, 0x0C060504);

			currentEmoteLayout = { 1, 2, 7, 10, 8, 3, 4 };
			useBlankEmote = true;

			initGraphicsRefresh(currentLink);
		}
	}

	// TODO: add blank emote?
	void lobbyEmoteSwapper(MenuEntry* entry) { 
		// disable default drablands emotes
		Process::Patch(AddressList::UseDoTLayoutAlways.addr, 0xEA00000D);

		u8 currentArea;
		Process::Read8(AddressList::CurrLevelID.addr, currentArea);

		if (currentArea == 0x1) {
			// get active link color
			u8 currentLink;
			Process::Read8(AddressList::ActiveLink.addr, currentLink);

			// layout A: over here, bye, lets go, hello, cheer, frown, no, thumbs up
			if (entry->Hotkeys[0].IsPressed()) {
				Process::Patch(AddressList::LobbyEmotes.addr, 0x04090100); // edit button IDs
				Process::Patch(AddressList::LobbyEmotes.addr + 0x4, 0x0C060302); // edit button IDs
				currentEmoteLayout = { 0, 1, 10, 8, 2, 7, 4 }; // config pointers

				useBlankEmote = true;
				initGraphicsRefresh(currentLink);
			}

			// layout B: item, throw, totem, yawn, cheer, frown, nooo, blank
			else if (entry->Hotkeys[1].IsPressed()) {
				Process::Patch(AddressList::LobbyEmotes.addr, 0x05080A0B);
				Process::Patch(AddressList::LobbyEmotes.addr + 0x4, 0x0C000706);
				currentEmoteLayout = { 11, 6, 5, 3, 4, 9, 0 }; // config pointers

				useBlankEmote = true;
				initGraphicsRefresh(currentLink);
			}
		}
	}

	void customEmotes(MenuEntry* entry) {
		// TODO: 
		// 1) verify .ctpk edits 
		// 2) TBD
	}
} 