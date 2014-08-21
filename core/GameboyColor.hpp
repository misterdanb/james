#ifndef _GameboyColor_hpp_
#define _GameboyColor_hpp_

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

#include "James.hpp"
#include "LCD.hpp"
#include "Joypad.hpp"
#include "cartridges/Cartridge.hpp"
#include "cpu/Processor.hpp"
#include "MemoryBus.hpp"
#include "LCDMode.hpp"
#include "BackgroundAttribute.hpp"
#include "SpriteAttribute.hpp"
#include "Tile.hpp"
#include "TileMap.hpp"
#include "RenderContext.hpp"
#include "ClassicRenderer.hpp"

using namespace james;
using namespace james::core;
using namespace james::core::cpu;
using namespace james::core::cartridges;

namespace james
{
	namespace core
	{
		class GameboyColor : public MemoryBus
		{
		  public:
			// clock speed over 256 will cause timer inaccuracies
			static const int CLOCK_SPEED = 1;

		  public:
			GameboyColor();
			~GameboyColor();

			void SetLCD (LCD&);
			void SetJoypad (Joypad&);
			void SetRom (DynamicArray<int>&);

			IInterruptHandler& GetInterruptHandler();
			Cartridge& GetCartridge();
			Processor& GetProcessor();
			Renderer& GetRenderer();

			TileMap::TileMapArray2& GetTileMap (int);
			SpriteAttribute& GetSpriteAttribute (int);

			void Initialize();
			void Finalize();

			void Start();
			void Pause();

			bool IsPaused();

			void Reset();

			void RenderScanline();
			void RenderFrame();

			void ExecuteMachineClocks (int);
			void UpdateTimer (int);

			int ReadByte (int);
			void WriteByte (int, int);

		  private:
			GameboyColor (GameboyColor&);
			GameboyColor& operator= (GameboyColor&);
			// emulation state
			bool _paused;
			int _pendingClocks;

			// lcd
			LCD* _lcd;

			// joypad
			Joypad* _joypad;

			int _directionKeysSelected;
			int _buttonKeysSelected;

			// cartridge
			Cartridge* _cartridge;
			int _forceClassicGameboy;

			// processor
			Processor _hybr1s80;
			int _speedFactor;

			Array<Color<int>, 4> _monochromePalette;

			int _colorBackgroundPaletteIndexAutoIncrement;
			int _colorSpritePaletteIndexAutoIncrement;

			Renderer* _renderer;

			RenderContext _rc;

			GameboyClassicSpecificRenderContext& _rcClassic = _rc.gameboyClassicSpecific;
			GameboyColorSpecificRenderContext& _rcColor = _rc.gameboyColorSpecific;
		};
	}
}

#endif
