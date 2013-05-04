#ifndef _RenderContext_hpp_
#define _RenderContext_hpp_

#include <vector>
#include "gbc.hpp"
#include "MemoryBus.hpp"
#include "InterruptHandler.hpp"
#include "Tile.hpp"
#include "TileMap.hpp"
#include "TileMapAttribute.hpp"
#include "InterruptHandler.hpp"
#include "LCDMode.hpp"
#include "Frame.hpp"
#include "SpriteAttribute.hpp"
#include "ColorPalette.hpp"

namespace gbc
{
	namespace core
	{
		class RenderContext
		{
		public:
			// memory dimensions
			static const int VIDEO_RAM_BANKS = 2;
			static const int VIDEO_RAM_BANK_SIZE = 0x2000;
			
			static const int WORK_RAM_BANKS = 8;
			static const int WORK_RAM_BANK_SIZE = 0x1000;
			
			static const int OAM_SIZE = 0x80;
			
			static const int IO_PORTS_SIZE = 0x80;
			
			static const int HIGH_RAM_SIZE = 0x80;
		
		public:
			// ram
			int videoRam[VIDEO_RAM_BANKS][VIDEO_RAM_BANK_SIZE];
			int workRam[WORK_RAM_BANKS][WORK_RAM_BANK_SIZE];
			int oam[OAM_SIZE];
			int ioPorts[IO_PORTS_SIZE];
			int highRam[HIGH_RAM_SIZE];
			int interruptEnableRegister;
			
			// ram banks
			int selectedWorkRamBank;
			int selectedVideoRamBank;
			
			// memory bus
			IMemoryBus *memoryBus;
			
			// interrupt handler
			IInterruptHandler *interruptHandler;
			
			// current scanline
			Color rawFrame[Frame::WIDTH * Frame::HEIGHT];
			
			// interrupt requests
			int verticalBlankInterruptAlreadyRequested;
			
			// interrupt enablers
			int verticalBlankInterruptEnabled;
			int lcdStatusInterruptEnabled;
			int timerInterruptEnabled;
			int serialInterruptEnabled;
			int joypadInterruptEnabled;
			
			// lcd control
			int lcdDisplayEnabled;
			int windowTileMapDisplaySelect;
			int windowDisplayEnabled;
			int backgroundAndWindowTileDataSelect;
			int backgroundTileMapDisplaySelect;
			int spriteSize;
			int spriteDisplayEnabled;
			int backgroundDisplayEnabled;
			
			// lcd status
			LCDMode lcdMode;
			int coincidenceFlag;
			int horizontalBlankInterruptEnabled;
			int verticalBlankInterruptEnabledInLCD;
			int oamInterruptEnabled;
			int coincidenceInterruptEnabled;
			
			// lcd position and scrolling
			int scrollX;
			int scrollY;
			int lcdY;
			int lcdYCompare;
			int windowX;
			int windowY;
			
			// tile data
			Tile tiles[2][384] = { { Tile(), }, }; // test
			std::vector<int *> changedTiles;
			
			// background map elements
			TileMap tileMaps[2];
			std::vector<int *> changedTileMapElements;
			
			// sprite attributes
			SpriteAttribute spriteAttributes[40];
			std::vector<int> changedSpriteAttributes;
			
			struct GameboyClassicSpecificRenderContext
			{
				// lcd monochrome palettes
				ColorPalette monochromeBackgroundPalette;
				ColorPalette monochromeSpritePalette0;
				ColorPalette monochromeSpritePalette1;
			} gameboyClassicSpecific;
			
			struct GameboyColorSpecificRenderContext
			{
				// background map attributes
				TileMapAttribute tileMapAttributes[2][TileMap::WIDTH * TileMap::HEIGHT];
				std::vector<int *> changedTileMapAttributes;
				
				// lcd color palettes
				ColorPalette colorBackgroundPalettes[8];
				ColorPalette colorSpritePalettes[8];
				
				// hblank dma transfer
				int dmaTransferActive;
				int dmaTransferSourceAddress;
				int dmaTransferDestinationAddress;
				int dmaTransferLength;
				int currentDMATransferOffset;
			} gameboyColorSpecific;
		};
	}
}

#endif
