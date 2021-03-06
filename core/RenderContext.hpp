#ifndef _RenderContext_hpp_
#define _RenderContext_hpp_

#include "James.hpp"
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

namespace james
{
	namespace core
	{
		class GameboyClassicSpecificRenderContext
		{
		  public:
			GameboyClassicSpecificRenderContext();
			~GameboyClassicSpecificRenderContext();
			ColorPalette monochromeBackgroundPalette;
			ColorPalette monochromeSpritePalette0;
			ColorPalette monochromeSpritePalette1;
		  private:
			GameboyClassicSpecificRenderContext (GameboyClassicSpecificRenderContext&) = delete;
			GameboyClassicSpecificRenderContext& operator= (GameboyClassicSpecificRenderContext&) = delete;
		};
		class GameboyColorSpecificRenderContext
		{
		  public:
			GameboyColorSpecificRenderContext();
			~GameboyColorSpecificRenderContext();
			TileMapAttribute tileMapAttributes[2][TileMap::WIDTH * TileMap::HEIGHT];
			Queue<Pair<int, int>>changedTileMapAttributes;
			ColorPalette colorBackgroundPalettes[8];
			ColorPalette colorSpritePalettes[8];
			int dmaTransferActive;
			int dmaTransferSourceAddress;
			int dmaTransferDestinationAddress;
			int dmaTransferLength;
			int currentDMATransferOffset;
		  private:
			GameboyColorSpecificRenderContext (GameboyColorSpecificRenderContext&) = delete;
			GameboyColorSpecificRenderContext& operator= (GameboyColorSpecificRenderContext&) = delete;
		};
		class RenderContext
		{
		  public:
			RenderContext();
			~RenderContext();
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
			Array2<int, VIDEO_RAM_BANKS, VIDEO_RAM_BANK_SIZE> videoRam;
			Array2<int, WORK_RAM_BANKS, WORK_RAM_BANK_SIZE> workRam;
			Array<int, OAM_SIZE> oam;
			Array<int, IO_PORTS_SIZE> ioPorts;
			Array<int, HIGH_RAM_SIZE> highRam;
			int interruptEnableRegister;

			// ram banks
			int selectedWorkRamBank;
			int selectedVideoRamBank;

			// memory bus
			IMemoryBus* memoryBus;

			// interrupt handler
			IInterruptHandler* interruptHandler;

			// current scanline
			Color<int> rawFrame[Frame::WIDTH * Frame::HEIGHT];

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
			Queue<Pair<int, int>> changedTiles;

			// background map elements
			TileMap tileMaps[2];
			Queue<Pair<int, int>> changedTileMapElements;

			// sprite attributes
			Array<SpriteAttribute, 40> spriteAttributes;
			Queue<int> changedSpriteAttributes;

			GameboyClassicSpecificRenderContext gameboyClassicSpecific;
			GameboyColorSpecificRenderContext gameboyColorSpecific;

		  private:
			RenderContext (RenderContext&) = delete;
			RenderContext& operator= (RenderContext&) = delete;

		};
	}
}

#endif
