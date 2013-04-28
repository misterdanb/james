#ifndef _GameboyColor_hpp_
#define _GameboyColor_hpp_

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

#include "gbc.hpp"
#include "LCD.hpp"
#include "Joypad.hpp"
#include "cartridges/Cartridge.hpp"
#include "cpu/Processor.hpp"
#include "MemoryBus.hpp"
#include "LCDMode.hpp"
#include "TileMapAttribute.hpp"
#include "SpriteAttribute.hpp"
#include "Tile.hpp"
#include "TileMap.hpp"
#include "Color.hpp"
#include "ColorPalette.hpp"

namespace gbc
{
	namespace core
	{
		using namespace cpu;
		using namespace cartridges;
		
		class GameboyColor : public IMemoryBus
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
			
			// color enable masks
			static const int COLOR_0 = 0b00000001;
			static const int COLOR_1 = 0b00000010;
			static const int COLOR_2 = 0b00000100;
			static const int COLOR_3 = 0b00001000;
		
		public:
			GameboyColor();
			~GameboyColor();
			
			void SetLCD(ILCD *);
			void SetJoypad(IJoypad *);
			void SetRom(int[], int);
			
			IInterruptHandler *GetInterruptHandler();
			
			void Initialize();
			
			void RenderScanline();
			void RenderFrame();
			
			void ExecuteMachineClocks(int);
			
			int ReadByte(int);
			void WriteByte(int, int);
			
		private:
			void DoOAMSearch();
			void DoTransferData();
			void DoHBlank();
			void DoVBlank();
			
			void UpdateTiles();
			void UpdateBackgroundMapElements();
			void UpdateTileMapAttributes();
			void UpdateSpriteAttributes();
			
			void DrawSprites(int, SpriteToBackgroundPriority);
			void DrawSprites(int);
			void DrawBackgroundMap(int, BackgroundToOAMPriority);
			void DrawBackgroundMap(int);
			void DrawWindowMap(int, BackgroundToOAMPriority);
			void DrawWindowMap(int);
			void DrawMapTile(int, int, int, int, int, PlatformSupport);
			void DrawTile(int, int, Tile, HorizontalFlip, VerticalFlip, ColorPalette, int);
			
			// current scanline
			Color _rawFrame[Frame::WIDTH * Frame::HEIGHT];
			
			// lcd
			ILCD *_lcd;
			
			// joypad
			IJoypad *_joypad;
			
			int _directionKeysSelected;
			int _buttonKeysSelected;
			
			// cartridge
			Cartridge *_cartridge;
			int _forceClassicGameboy;
			
			// processor
			Processor _hybr1s80;
			int _speedFactor;
			
			// ram
			int _videoRam[VIDEO_RAM_BANKS][VIDEO_RAM_BANK_SIZE];
			int _workRam[WORK_RAM_BANKS][WORK_RAM_BANK_SIZE];
			int _oam[OAM_SIZE];
			int _ioPorts[IO_PORTS_SIZE];
			int _highRam[HIGH_RAM_SIZE];
			int _interruptEnableRegister;
			
			// ram banks
			int _selectedWorkRamBank;
			int _selectedVideoRamBank;
			
			// timer
			int _timerClockFrequency;
			int _timerStopped;
			int _deviderCounter;
			int _timerCounter;
			
			// interrupt requests
			int _vBlankInterruptAlreadyRequested;
			
			// interrupt enablers
			int _vBlankInterruptEnabled;
			int _lcdStatusInterruptEnabled;
			int _timerInterruptEnabled;
			int _serialInterruptEnabled;
			int _joypadInterruptEnabled;
			
			// lcd control
			int _lcdDisplayEnabled;
			int _windowTileMapDisplaySelect;
			int _windowDisplayEnabled;
			int _backgroundAndWindowTileDataSelect;
			int _backgroundTileMapDisplaySelect;
			int _spriteSize;
			int _spriteDisplayEnabled;
			int _backgroundDisplayEnabled;
			
			// lcd status
			LCDMode _lcdMode;
			int _coincidenceFlag;
			int _hBlankInterruptEnabled;
			int _vBlankInterruptEnabledInLCD;
			int _oamInterruptEnabled;
			int _coincidenceInterruptEnabled;
			
			// lcd position and scrolling
			int _scrollX;
			int _scrollY;
			int _lcdY;
			int _lcdYCompare;
			int _windowY;
			int _windowX;
			
			// lcd monochrome palettes
			ColorPalette _monochromeBackgroundPalette;
			ColorPalette _monochromeSpritePalette0;
			ColorPalette _monochromeSpritePalette1;
			ColorPalette _monochromePalette;
			
			// lcd color palettes
			int _colorBackgroundPaletteIndexAutoIncrement;
			ColorPalette _colorBackgroundPalettes[8];
			int _colorSpritePaletteIndexAutoIncrement;
			ColorPalette _colorSpritePalettes[8];
			
			// tile data
			Tile _tiles[2][384];
			std::vector<int *> _changedTiles; // (tile vram bank, tile number)
			
			// background map elements
			TileMap _tileMaps[2];
			std::vector<int *> _changedBackgroundMapElements;
			
			// background map attributes
			TileMapAttribute _backgroundMapAttributes[2][32 * 32];
			std::vector<int *> _changedTileMapAttributes;
			
			// sprite attributes
			SpriteAttribute _spriteAttributes[40];
			std::vector<int> _changedSpriteAttributes;
			
			// hblank dma transfer
			int _hBlankDMATransferActive;
			int _hBlankDMATransferSourceAddress;
			int _hBlankDMATransferDestinationAddress;
			int _hBlankDMATransferLength;
			int _currentHBlankDMATransferAddressOffset;
		};
	}
}

#endif
