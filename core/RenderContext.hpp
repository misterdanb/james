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
			// interrupt handler
			IInterruptHandler *interruptHandler;
			
			// interrupt requests
			int vBlankInterruptAlreadyRequested;
			
			// interrupt enablers
			int vBlankInterruptEnabled;
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
			int windowY;
			int windowX;
			
			// tile data
			Tile tiles[2][384];
			
			// background map elements
			TileMap tileMaps[2];
			
			// sprite attributes
			SpriteAttribute spriteAttributes[40];
			std::vector<int> changedSpriteAttributes;
			
			struct
			{
				// lcd monochrome palettes
				ColorPalette monochromeBackgroundPalette;
				ColorPalette monochromeSpritePalette0;
				ColorPalette monochromeSpritePalette1;
				ColorPalette monochromePalette;
			} gameboySpecific;
			
			struct
			{
				// background map attributes
				TileMapAttribute backgroundMapAttributes[2][32 * 32];
				
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
