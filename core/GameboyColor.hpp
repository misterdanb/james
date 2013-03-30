#ifndef _GameboyColor_hpp_
#define _GameboyColor_hpp_

#include <iostream>
#include <vector>
#include <algorithm>

#include "GBC.hpp"
#include "LCD.hpp"
#include "cartridges/Cartridge.hpp"
#include "cpu/Processor.hpp"
#include "MemoryBus.hpp"
#include "LCDMode.hpp"
#include "BackgroundMapAttribute.hpp"
#include "SpriteAttribute.hpp"
#include "Color.hpp"

#define VRAM_BANKS 2
#define VRAM_BANK_SIZE 0x2000

#define WRAM_BANKS 8
#define WRAM_BANK_SIZE 0x1000

#define OAM_SIZE 0x80

#define IOPORTS_SIZE 0x80

#define HRAM_SIZE 0x80

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
			void SetRom(int[]);
			
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
			void UpdateBackgroundMapAttributes();
			void UpdateSpriteAttributes();
			
			// lcd
			ILCD *_lcd;
			
			// cartridge
			cartridges::Cartridge *_cartridge;
			
			// processor
			cpu::Processor _wtf80;
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
			int _monochromePalette[4];
			
			// lcd color phunalettes
			int _colorBackgroundPaletteIndexAutoIncrement;
			Color _colorBackgroundPalettes[8][4];
			int _colorSpritePaletteIndexAutoIncrement;
			Color _colorSpritePalettes[8][4];
			
			// tile data
			int _tiles[2][384][8][8];
			std::vector<int *> _changedTiles; // (tile vram bank, tile number)
			
			// bg map attributes
			BackgroundMapAttribute _backgroundMapAttributes[0x0800];
			std::vector<int> _changedBackgroundMapAttributes;
			
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
