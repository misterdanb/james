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
#include "RenderContext.hpp"
#include "ClassicRenderer.hpp"

namespace gbc
{
	namespace core
	{
		using namespace cpu;
		using namespace cartridges;
		
		class GameboyColor : public IMemoryBus
		{
		/*public:
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
			static const int COLOR_3 = 0b00001000;*/
		
		public:
			GameboyColor();
			~GameboyColor();
			
			void SetLCD(ILCD *);
			void SetJoypad(IJoypad *);
			void SetRom(DynamicArray<int> &);
			
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
			
			// timer
			int _timerClockFrequency;
			int _timerStopped;
			int _deviderCounter;
			int _timerCounter;
			
			ColorPalette _monochromePalette;
			
			int _colorBackgroundPaletteIndexAutoIncrement;
			int _colorSpritePaletteIndexAutoIncrement;
			
			Renderer *_renderer;
			
			RenderContext _rc;
			
			RenderContext::GameboyClassicSpecificRenderContext &_rcClassic = _rc.gameboyClassicSpecific;
			RenderContext::GameboyColorSpecificRenderContext &_rcColor = _rc.gameboyColorSpecific;
		};
	}
}

#endif
