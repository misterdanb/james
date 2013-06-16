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
#include "ColorPalette.hpp"
#include "RenderContext.hpp"
#include "ClassicRenderer.hpp"

using namespace gbc;
using namespace gbc::core;
using namespace gbc::core::cpu;
using namespace gbc::core::cartridges;

namespace gbc
{
	namespace core
	{
		class GameboyColor : public IMemoryBus
		{
		public:
			GameboyColor();
			~GameboyColor();
			
			void SetLCD(ILCD &);
			void SetJoypad(IJoypad &);
			void SetRom(DynamicArray<int> &);
			
			IInterruptHandler &GetInterruptHandler();
			Renderer &GetRenderer();
			
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
