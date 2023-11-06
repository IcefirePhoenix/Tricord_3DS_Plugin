#include "cheats.hpp"
#include "AddressList.hpp"
#include "Helpers/Address.hpp"
#include "3ds.h"

namespace CTRPluginFramework
{
	u32 blankEmotePointer = 0x2070A780;

	const u32 greenEmotePointers[] = {
		0x20307780, // over here
		0x20308780, // item
		0x20309780, // throw
		0x2030A780, // shrug/bow
		0x2030B780,	// no
		0x2030C780, // hello
		0x2030D780,	// lets go
		0x20C86080, // totem
		0x20C87080, // cheer
		0x20C88080,	// thumbs up
		0x20C89080, // yawn
		0x20C8A080, // bye
	};

	const u32 blueEmotePointers[] = {
		0x20300780, // over here
		0x20301780, // item
		0x20302780, // throw
		0x20303780, // shrug/bow
		0x20304780,	// no
		0x20305780, // hello
		0x20306780,	// lets go
		0x20C81080, // totem
		0x20C82080, // cheer
		0x20C83080,	// thumbs up
		0x20C84080, // yawn
		0x20C85080, // bye
	};

	const u32 redEmotePointers[] = {
		0x2030E780, // over here
		0x2030F780, // item
		0x20310780, // throw
		0x20311780, // frown/bow
		0x20312780,	// no
		0x20313780, // hello
		0x20314780,	// lets go
		0x20C8B080, // totem
		0x20C8C080, // cheer
		0x20C8D080,	// thumbs up
		0x20C8E080, // yawn
		0x20C8F080, // bye
	};

	std::vector<u8> currentEmoteLayout;

	void refreshEmoteGraphics(const u32 *color, std::vector<u8> &layout, bool useBlank) {
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

		// write graphic pointers
		for (int i = 0; i < 8; i++) {
			if (useBlank && i == 7) {
				Process::Write32(graphicsAddresses[i], blankEmotePointer);
			}
			else {
				Process::Write32(graphicsAddresses[i], color[layout[i]]);
			}
		}
	}

	void drablandEmoteSwapper(MenuEntry* entry) {
		// disable default drablands emotes
		Process::Patch(AddressList::UseDoTLayoutAlways.addr, 0xEA00000D);

		// get active link color
		u8 currentLink;
		Process::Read8(AddressList::ActiveLink.addr, currentLink);

		// layout A: over here, bye, lets go, hello, cheer, frown, no, thumbs up
		if (entry->Hotkeys[0].IsPressed()) {
			Process::Patch(AddressList::GameplayEmotes.addr, 0x080A0B00); // edit button IDs
			Process::Patch(AddressList::GameplayEmotes.addr + 0x4, 0x07060504); // edit button IDs
			currentEmoteLayout = { 0, 11, 6, 5, 8, 3, 4, 9 }; // config pointers

			// manually refresh graphics
			switch (currentLink) {
			case 0:
				refreshEmoteGraphics(greenEmotePointers, currentEmoteLayout, false);
				break;
			case 1:
				refreshEmoteGraphics(blueEmotePointers, currentEmoteLayout, false);
				break;
			case 2:
				refreshEmoteGraphics(redEmotePointers, currentEmoteLayout, false);
				break;
			}
		}
		// layout B: item, throw, totem, yawn, cheer, frown, nooo, blank
		else if (entry->Hotkeys[1].IsPressed()) {
			Process::Patch(AddressList::GameplayEmotes.addr, 0x09030201);
			Process::Patch(AddressList::GameplayEmotes.addr + 0x4, 0x0C060504);

			currentEmoteLayout = { 1, 2, 7, 10, 8, 3, 4 }; // config pointers

			// manually refresh graphics
			switch (currentLink) {
			case 0:
				refreshEmoteGraphics(greenEmotePointers, currentEmoteLayout, true);
				break;
			case 1:
				refreshEmoteGraphics(blueEmotePointers, currentEmoteLayout, true);
				break;
			case 2:
				refreshEmoteGraphics(redEmotePointers, currentEmoteLayout, true);
				break;
			}
		}
	}

	void lobbyEmoteSwapper(MenuEntry* entry) {
		// TODO: 
		// 1) rewrite emote swapper...
	}

	void customEmotes(MenuEntry* entry) {
		// TODO: 
		// 1) verify .ctpk edits 
		// 2) TBD
	}
} 