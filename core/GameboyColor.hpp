#ifndef _GameboyColor_hpp_
#define _GameboyColor_hpp_

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

#include "GBC.hpp"
#include "LCD.hpp"
#include "Joypad.hpp"
#include "cartridges/Cartridge.hpp"
#include "cpu/Processor.hpp"
#include "MemoryBus.hpp"
#include "LCDMode.hpp"
#include "BackgroundMapAttribute.hpp"
#include "SpriteAttribute.hpp"
#include "Tile.hpp"
#include "Color.hpp"
#include "ColorPalette.hpp"

#define VRAM_BANKS 2
#define VRAM_BANK_SIZE 0x2000

#define WRAM_BANKS 8
#define WRAM_BANK_SIZE 0x1000

#define OAM_SIZE 0x80

#define IOPORTS_SIZE 0x80

#define HRAM_SIZE 0x80

#define COLOR_0 0b00000001
#define COLOR_1 0b00000010
#define COLOR_2 0b00000100
#define COLOR_3 0b00001000

namespace gbc
{
	namespace core
	{
		class GameboyColor : public IMemoryBus
		{
		public:
			GameboyColor();
			~GameboyColor();
			
			void Initialize();
			void SetLCD(ILCD *);
			void SetJoypad(IJoypad *);
			void SetRom(int[], int);
			
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
			void UpdateBackgroundMapAttributes();
			void UpdateSpriteAttributes();
			
			void DrawSprites(int, SpriteToBackgroundPriority);
			void DrawSprites(int);
			void DrawBackgroundMap(int, BackgroundToOAMPriority);
			void DrawBackgroundMap(int);
			void DrawWindowMap(int, BackgroundToOAMPriority);
			void DrawWindowMap(int);
			void DrawMapTile(int, int, int, int, int, cartridges::PlatformSupport);
			void DrawTile(int, int, Tile, HorizontalFlip, VerticalFlip, ColorPalette, int);
			
			// current scanline
			Color _currentScanline[160];
			
			// lcd
			ILCD *_lcd;
			
			// joypad
			IJoypad *_joypad;
			
			int _directionKeysSelected;
			int _buttonKeysSelected;
			
			// cartridge
			cartridges::Cartridge *_cartridge;
			int _forceClassicGameboy;
			
			// processor
			cpu::Processor _hybr1s80;
			int _speedFactor;
			
			// ram
			int _videoRam[VRAM_BANKS][VRAM_BANK_SIZE];
			int _workRam[WRAM_BANKS][WRAM_BANK_SIZE];
			int _oam[OAM_SIZE];
			int _ioPorts[IOPORTS_SIZE];
			int _highRam[HRAM_SIZE];
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
			int _vBlankInterruptRequested;
			int _lcdStatusInterruptRequested;
			int _timerInterruptRequested;
			int _serialInterruptRequested;
			int _joypadInterruptRequested;
			
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
			int _monochromeBackgroundPaletteData[4];
			int _monochromeSpritePalette0Data[4];
			int _monochromeSpritePalette1Data[4];
			ColorPalette _monochromePalette;
			
			// lcd color phunalettes
			int _colorBackgroundPaletteIndexAutoIncrement;
			ColorPalette _colorBackgroundPalettes[8];
			int _colorSpritePaletteIndexAutoIncrement;
			ColorPalette _colorSpritePalettes[8];
			
			// tile data
			Tile _tiles[2][384];
			std::vector<int *> _changedTiles; // (tile vram bank, tile number)
			
			// background map elements
			int _backgroundMapElements[2][32 * 32];
			std::vector<int *> _changedBackgroundMapElements;
			
			// background map attributes
			BackgroundMapAttribute _backgroundMapAttributes[2][32 * 32];
			std::vector<int *> _changedBackgroundMapAttributes;
			
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
