#ifndef _RenderContext_hpp_
#define _RenderContext_hpp_

#include <vector>
#include "gbc.hpp"
#include "Tile.hpp"
#include "TileMap.hpp"
#include "InterruptHandler.hpp"
#include "LCDMode.hpp"
#include "SpriteAttribute.hpp"
#include "ColorPalette.hpp"

namespace gbc
{
	namespace core
	{
		class RenderContext
		{
		public:
			// memory bus
			IMemoryBus *memoryBus;
			
			// interrupt handler
			IInterruptHandler *interruptHandler;
			
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
			Tile tiles[2][384];
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
				int horizontalBlankDMATransferActive;
				int horizontalBlankDMATransferSourceAddress;
				int horizontalBlankDMATransferDestinationAddress;
				int horizontalBlankDMATransferLength;
				int currentHorizontalBlankDMATransferAddressOffset;
			} gameboyColorSpecific;
		};
	}
}

#endif
