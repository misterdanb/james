#include "GameboyColor.hpp"

GameboyColor::GameboyColor()
	: _paused (true),
	  _pendingClocks (0),
	  _lcd (NULL),
	  _joypad (NULL),
	  _directionKeysSelected (0),
	  _buttonKeysSelected (1),
	  _cartridge (NULL),
	  _forceClassicGameboy (GBC_TRUE),
	  _timer(),
	  _hybr1s80(),
	  _speedFactor (1),
	  _monochromePalette(),
	  _colorBackgroundPaletteIndexAutoIncrement (0),
	  _colorSpritePaletteIndexAutoIncrement (0),
	  _renderer(),
	  _rc()
{
	_rc.memoryBus = this;
	_rc.interruptHandler = &_hybr1s80;

	_rc.interruptEnableRegister = 0x00;
	_rc.selectedWorkRamBank = 1; // 0xD000 is 1-7
	_rc.selectedVideoRamBank = 0;
	_rc.verticalBlankInterruptAlreadyRequested = GBC_FALSE;
	_rc.lcdDisplayEnabled = GBC_FALSE;
	_rc.windowMapDisplaySelect = 0;
	_rc.windowDisplayEnabled = GBC_FALSE;
	_rc.backgroundAndWindowTileDataSelect = 0;
	_rc.backgroundMapDisplaySelect = 0;
	_rc.spriteSize = 0; // missing enum for sprite size (and implementation of this)
	_rc.spriteDisplayEnabled = GBC_FALSE;
	_rc.backgroundDisplayEnabled = GBC_FALSE;
	_rc.lcdMode = LCDMode::VERTICAL_BLANK;
	_rc.coincidenceFlag = GBC_FALSE;
	_rc.horizontalBlankInterruptEnabled = GBC_FALSE;
	_rc.verticalBlankInterruptEnabledInLCD = GBC_FALSE;
	_rc.oamInterruptEnabled = GBC_FALSE;
	_rc.coincidenceInterruptEnabled = GBC_FALSE;
	_rc.scrollX = 0;
	_rc.scrollY = 0;
	_rc.lcdY = 0;
	_rc.lcdYCompare = GBC_FALSE;
	_rc.windowX = 0;
	_rc.windowY = 0;
	_rcColor.dmaTransferActive = GBC_FALSE;
	_rcColor.dmaTransferSourceAddress = 0x0000;
	_rcColor.dmaTransferDestinationAddress = 0x0000;
	_rcColor.dmaTransferLength = 0;
	_rcColor.currentDMATransferOffset = 0x0000;
}

GameboyColor::~GameboyColor()
{
	delete _renderer;
	delete _cartridge;
}

void GameboyColor::SetLCD (LCD& lcd)
{
	_lcd = &lcd;
}

void GameboyColor::SetJoypad (Joypad& joypad)
{
	_joypad = &joypad;
}

void GameboyColor::SetRom (DynamicArray<int>& rom)
{
	_cartridge = Cartridge::Create (rom);

	std::string path = ToHex (_cartridge->GetHeader().globalChecksum[0]) +
					   ToHex (_cartridge->GetHeader().globalChecksum[1]) +
					   std::string (".battery");

	std::ifstream file (path, std::ios::binary);

	if (file.is_open() && !file.bad())
	{
		file >> (*_cartridge);
		file.close();

		LOG ("Loaded cartridge ram battery");
	}
	else
	{
		ERROR ("Failed to load cartridge ram battery");
	}

	LOG ("Loaded cartridge");

	if (_cartridge->GetHeader().platformSupport == PlatformSupport::GAMEBOY_COLOR_SUPPORT ||
			_cartridge->GetHeader().platformSupport != PlatformSupport::GAMEBOY_COLOR_ONLY)
	{
		_renderer = new ClassicRenderer (_rc);

		LOG ("Using Gameboy Classic rendering method");
	}
}

IInterruptHandler& GameboyColor::GetInterruptHandler()
{
	return _hybr1s80;
}

Cartridge& GameboyColor::GetCartridge()
{
	return *_cartridge;
}

Processor& GameboyColor::GetProcessor()
{
	return _hybr1s80;
}

Renderer& GameboyColor::GetRenderer()
{
	return (*_renderer);
}

TileMap::TileMapArray2& GameboyColor::GetTileMap (int tileMapNumber)
{
	return _rc.backgroundMaps[tileMapNumber].data;
}

SpriteAttribute& GameboyColor::GetSpriteAttribute (int spriteAttributeNumber)
{
	return _rc.spriteAttributes[spriteAttributeNumber];
}

void GameboyColor::Initialize()
{
	LOG ("Initializing Gameboy Color emulation");

	_timer.SetRenderContext (_rc);

	_hybr1s80.SetMemoryBus (this);

	Reset();

	_monochromePalette[3].SetRed (0x00);
	_monochromePalette[3].SetGreen (0x00);
	_monochromePalette[3].SetBlue (0x00);
	_monochromePalette[2].SetRed (0x08);
	_monochromePalette[2].SetGreen (0x08);
	_monochromePalette[2].SetBlue (0x08);
	_monochromePalette[1].SetRed (0x10);
	_monochromePalette[1].SetGreen (0x10);
	_monochromePalette[1].SetBlue (0x10);
	_monochromePalette[0].SetRed (0x1F);
	_monochromePalette[0].SetGreen (0x1F);
	_monochromePalette[0].SetBlue (0x1F);

	std::copy (_monochromePalette.begin(),
			   _monochromePalette.end(),
			   _rcClassic.monochromeBackgroundPalette.begin());

	std::copy (_monochromePalette.begin(),
			   _monochromePalette.end(),
			   _rcClassic.monochromeSpritePalette0.begin());

	std::copy (_monochromePalette.begin(),
			   _monochromePalette.end(),
			   _rcClassic.monochromeSpritePalette1.begin());
}

void GameboyColor::Finalize()
{
	LOG ("Finalizing Gameboy Color emulation");

	std::string path = ToHex (_cartridge->GetHeader().globalChecksum[0]) +
					   ToHex (_cartridge->GetHeader().globalChecksum[1]) +
					   std::string (".battery");

	std::ofstream file (path, std::ios::binary);

	if (file.is_open() && !file.bad())
	{
		file << (*_cartridge);
		file.close();

		LOG ("Saved cartridge ram battery");
	}
	else
	{
		ERROR ("Failed to save cartridge ram battery");
	}
}

void GameboyColor::Start()
{
	_paused = false;

	LOG ("Started emulation");
}

void GameboyColor::Pause()
{
	_paused = true;

	LOG ("Paused emulation");
}

bool GameboyColor::IsPaused()
{
	return _paused;
}

void GameboyColor::Reset()
{
	for (Array<int, RenderContext::VIDEO_RAM_BANK_SIZE>& videoRamBank : _rc.videoRam)
	{
		std::fill (videoRamBank.begin(), videoRamBank.end(), 0x00);
	}

	for (Array<int, RenderContext::WORK_RAM_BANK_SIZE>& workRamBank : _rc.workRam)
	{
		std::fill (workRamBank.begin(), workRamBank.end(), 0x00);
	}

	std::fill (_rc.oam.begin(), _rc.oam.end(), 0x00);
	std::fill (_rc.ioPorts.begin(), _rc.ioPorts.end(), 0x00);
	std::fill (_rc.highRam.begin(), _rc.highRam.end(), 0x00);

	_hybr1s80.PowerUp();
}

void GameboyColor::RenderScanline()
{
	if (_rc.lcdY < 144)
	{
		_rc.lcdMode = LCDMode::SEARCHING_OAM;
		ExecuteMachineClocks (80 * _speedFactor);
		_renderer->RenderOAMSearch();

		_rc.lcdMode = LCDMode::TRANSFERRING_DATA;
		ExecuteMachineClocks (172 * _speedFactor);
		_renderer->RenderTransferData();

		_rc.lcdMode = LCDMode::HORIZONTAL_BLANK;
		ExecuteMachineClocks (204 * _speedFactor);
		_renderer->RenderHorizontalBlank();
	}
	else
	{
		_rc.lcdMode = LCDMode::VERTICAL_BLANK;
		ExecuteMachineClocks (456 * _speedFactor);
		_renderer->RenderVerticalBlank();
	}
}

void GameboyColor::RenderFrame()
{
	if (!_paused)
	{
		for (int i = 0; i < 154; i++)
		{
			RenderScanline();
		}
	}

	if (_lcd)
	{
		Frame renderedFrame (_rc.rawFrame);

		_lcd->DrawFrame (renderedFrame);
	}
	else
	{
		ERROR ("Unable to draw a frame: No LCD set.");
	}
}

void GameboyColor::ExecuteMachineClocks (int clocks)
{
	_pendingClocks += clocks;

	while (_pendingClocks >= CLOCK_SPEED)
	{
		_pendingClocks -= CLOCK_SPEED;

		_timer.ExecuteTicks (CLOCK_SPEED * 4);
		_hybr1s80.ExecuteTicks (CLOCK_SPEED * 4);
	}
}

inline int GameboyColor::ReadByte (int address)
{
	// das geht eleganter...
	address = address & 0xFFFF;

	if (address <= 0x7FFF)
	{
		return _cartridge->ReadByte (address);
	}
	else if (address <= 0x9FFF)
	{
		return _rc.videoRam[_rc.selectedVideoRamBank][address - 0x8000];
	}
	else if (address <= 0xBFFF)
	{
		return _cartridge->ReadByte (address);
	}
	else if (address <= 0xCFFF)
	{
		return _rc.workRam[0][address - 0xC000];
	}
	else if (address <= 0xDFFF)
	{
		return _rc.workRam[_rc.selectedWorkRamBank][address - 0xD000];
	}
	else if (address <= 0xEFFF)
	{
		return _rc.workRam[0][address - 0xE000];
	}
	else if (address <= 0xFDFF)
	{
		return _rc.workRam[_rc.selectedWorkRamBank][address - 0xF000];
	}
	else if (address <= 0xFE9F)
	{
		return _rc.oam[address - 0xFE00];
	}
	else if (address <= 0xFEFF)
	{
		return 0x00;
	}
	else if (address <= 0xFF7F)
	{
		switch (address)
		{
			//case 0xFF00: return 0xFF;
			case 0xFF00:
				// joypad
				return ((! (_directionKeysSelected ? _joypad->GetRight() : _joypad->GetButtonA())) ? 1 : 0) |
					   ((! (_directionKeysSelected ? _joypad->GetLeft() : _joypad->GetButtonB())) ? 2 : 0) |
					   ((! (_directionKeysSelected ? _joypad->GetUp() : _joypad->GetSelect())) ? 4 : 0) |
					   ((! (_directionKeysSelected ? _joypad->GetDown() : _joypad->GetStart())) ? 8 : 0) |
					   ((!_directionKeysSelected) ? 16 : 0) |
					   ((!_buttonKeysSelected) ? 32 : 0);

			case 0xFF41:
				// lcd status
				return (_rc.ioPorts[0xFF41 - 0xFF00] & 0xF8) |
					   (GetEnumValue (_rc.lcdMode) & 0x03) |
					   (_rc.coincidenceFlag ? 0x04 : 0x00);

			case 0xFF44:
				// lcd y
				return _rc.lcdY;

			case 0xFF4F:
				// vram bank
				return _rc.selectedVideoRamBank;

			case 0xFF70:
				// wram bank
				return _rc.selectedWorkRamBank;

			default:
				return _rc.ioPorts[address - 0xFF00];
		}

	}
	else if (address <= 0xFFFE)
	{
		return _rc.highRam[address - 0xFF80];
	}
	else if (address == 0xFFFF)
	{
		// interrupt enable
		return _rc.interruptEnableRegister;
	}

	LOG (std::string ("MemoryMap: Address out of range: ") + ToHex (address));

	return 0x00;
}

inline void GameboyColor::WriteByte (int address, int value)
{
	// das geht eleganter...
	address &= 0xFFFF;
	value &= 0xFF;

	if (address >= 0x0000 && address <= 0x7FFF)
	{
		_cartridge->WriteByte (address, value);
	}
	else if (address <= 0x9FFF)
	{
		//		if (_rc.lcdMode != LCDMode::TRANSFERRING_DATA)
		{
			_rc.videoRam[_rc.selectedVideoRamBank][address - 0x8000] = value;

			if (address <= 0x97FF)
			{
				int tileRamPartAddress = (address - 0x8000) & 0xFFF0;
				int tileNumber = tileRamPartAddress >> 4;
				int y = (address % 0x10) / 2;

				Tile& tileToChange = _rc.tiles[_rc.selectedVideoRamBank][tileNumber];

				int colorNumbersLow = _rc.videoRam[_rc.selectedVideoRamBank][tileRamPartAddress + y * 2];
				int colorNumbersHigh = _rc.videoRam[_rc.selectedVideoRamBank][tileRamPartAddress + y * 2 + 1];

				switch (address % 2)
				{
					case 0:
						for (int x = 0; x < Tile::WIDTH; x++)
						{
							tileToChange[x][y] &= 2;
							tileToChange[x][y] |= (colorNumbersLow >> (7 - x)) & 1;
						}

						break;

					case 1:
						for (int x = 0; x < Tile::WIDTH; x++)
						{
							tileToChange[x][y] &= 1;
							tileToChange[x][y] |= ((colorNumbersHigh << 1) >> (7 - x)) & 2;
						}

						break;

					default:
						break;
				}
			}
			else
			{
				if (_rc.selectedVideoRamBank == 0)
				{
					int mapNumber = (address <= 0x9BFF) ? 0 : 1;
					int mapElementNumber = (address <= 0x9BFF) ? (address - 0x9800) : (address - 0x9C00);

					int mapElementX = mapElementNumber % TileMap::WIDTH;
					int mapElementY = mapElementNumber / TileMap::WIDTH;

					TileMap& backgroundMap = _rc.backgroundMaps[mapNumber];

					backgroundMap.data[mapElementX][mapElementY] = _rc.videoRam[0][ (mapNumber == 0) ?
							(0x9800 - 0x8000 + mapElementNumber) :
							(0x9C00 - 0x8000 + mapElementNumber)];
				}
				else
				{
					int mapNumber = (address <= 0x9BFF) ? 0 : 1;
					int mapElementNumber = (address <= 0x9BFF) ? (address - 0x9800) : (address - 0x9C00);

					int backgroundAttribute = _rc.videoRam[1][ (mapNumber == 0) ?
											  (0x9800 + mapElementNumber) :
											  (0x9C00 + mapElementNumber)];

					_rcColor.backgroundAttributes[mapNumber][mapElementNumber] = BackgroundAttribute (backgroundAttribute);
				}
			}
		}
	}
	else if (address <= 0xBFFF)
	{
		_cartridge->WriteByte (address, value);
	}
	else if (address <= 0xCFFF)
	{
		_rc.workRam[0][address - 0xC000] = value;
	}
	else if (address <= 0xDFFF)
	{
		_rc.workRam[_rc.selectedWorkRamBank][address - 0xD000] = value;
	}
	else if (address <= 0xEFFF)
	{
		_rc.workRam[0][address - 0xE000] = value;
	}
	else if (address <= 0xFDFF)
	{
		_rc.workRam[_rc.selectedWorkRamBank][address - 0xF000] = value;
	}
	else if (address <= 0xFE9F)
	{
		if (_rc.lcdMode != LCDMode::SEARCHING_OAM &&
				_rc.lcdMode != LCDMode::TRANSFERRING_DATA)
		{
			_rc.oam[address - 0xFE00] = value;

			int oamAddress = (address - 0xFE00) & 0xFC;

			Array<int, 4> spriteAttributeData =
			{
				_rc.oam[oamAddress],
				_rc.oam[oamAddress + 1],
				_rc.oam[oamAddress + 2],
				_rc.oam[oamAddress + 3]
			};

			_rc.spriteAttributes[oamAddress >> 2] = SpriteAttribute (spriteAttributeData);
		}
	}
	else if (address <= 0xFEFF)
	{
	}
	else if (address <= 0xFF7F)
	{
		switch (address)
		{
			case 0xFF00:
				// joypad input
				_rc.ioPorts[address - 0xFF00] = value;

				_directionKeysSelected = !GetBit (value, 4);
				_buttonKeysSelected = !GetBit (value, 5);

				break;

			case 0xFF01:
				// serial transfer data
				_rc.ioPorts[address - 0xFF00] = value;

				break;

			case 0xFF02:
				// serial transfer control
				_rc.ioPorts[address - 0xFF00] = value;

				break;

			case 0xFF04:
				// write to devider register
				_rc.ioPorts[address - 0xFF00] = 0;

				break;

			case 0xFF05:
				// timer counter
				_rc.ioPorts[address - 0xFF00] = value;

				break;

			case 0xFF06:
				// timer modulo
				_rc.ioPorts[address - 0xFF00] = value;

				break;

			case 0xFF07:
				// timer control
				_rc.ioPorts[address - 0xFF00] = value;

				break;

			case IInterruptHandler::INTERRUPT_REQUEST_ADDRESS:
				// interrupt request flags
				_rc.ioPorts[IInterruptHandler::INTERRUPT_REQUEST_ADDRESS - 0xFF00] = value;

				break;

			case 0xFF40:
				// lcd control
				_rc.ioPorts[address - 0xFF00] = value;

				_rc.backgroundDisplayEnabled = GetBit (value, 0);
				_rc.spriteDisplayEnabled = GetBit (value, 1);
				_rc.spriteSize = GetBit (value, 2);
				_rc.backgroundMapDisplaySelect = GetBit (value, 3);
				_rc.backgroundAndWindowTileDataSelect = GetBit (value, 4);
				_rc.windowDisplayEnabled = GetBit (value, 5);
				_rc.windowMapDisplaySelect = GetBit (value, 6);
				_rc.lcdDisplayEnabled = GetBit (value, 7);

				break;

			case 0xFF41:
				// lcd status register
				// doesn't have any effect
				_rc.ioPorts[address - 0xFF00] = value;

				//_rc.lcdMode = LCDMode(value & 0x03);
				//_rc.coincidenceFlag = GetBit(value, 2);
				_rc.horizontalBlankInterruptEnabled = GetBit (value, 3);
				_rc.verticalBlankInterruptEnabledInLCD = GetBit (value, 4);
				_rc.oamInterruptEnabled = GetBit (value, 5);
				_rc.coincidenceInterruptEnabled = GetBit (value, 6);

				break;

			case 0xFF42:
				// scroll y
				_rc.ioPorts[address - 0xFF00] = value;
				_rc.scrollY = value;

				break;

			case 0xFF43:
				// scroll x
				_rc.ioPorts[address - 0xFF00] = value;
				_rc.scrollX = value;

				break;

			case 0xFF44:
				// lcd y coordinate
				// doesn't have any effect
				_rc.ioPorts[address - 0xFF00] = value;

				// reset counter
				_rc.lcdY = 0;

				break;

			case 0xFF45:
				// lcd y compare
				_rc.ioPorts[address - 0xFF00] = value;
				_rc.lcdYCompare = value;

				break;

			case 0xFF46:
				// dma to oam transfer
				_rc.ioPorts[address - 0xFF00] = value;

				for (int i = 0; i < 0x0100; i++)
				{
					WriteByte (0xFE00 + i, ReadByte ((value << 8) + i));
				}

				break;

			case 0xFF47:
				// background palette data
				_rc.ioPorts[address - 0xFF00] = value;

				// TODO: CONVERT ALL THESE DECIMALS TO HEX
				_rcClassic.monochromeBackgroundPalette[0] = _monochromePalette[value & 3];
				_rcClassic.monochromeBackgroundPalette[1] = _monochromePalette[ (value & 12) >> 2];
				_rcClassic.monochromeBackgroundPalette[2] = _monochromePalette[ (value & 48) >> 4];
				_rcClassic.monochromeBackgroundPalette[3] = _monochromePalette[ (value & 192) >> 6];

				break;

			case 0xFF48:
				// sprite palette 0 data
				_rc.ioPorts[address - 0xFF00] = value;

				_rcClassic.monochromeSpritePalette0[0] = _monochromePalette[value & 3];
				_rcClassic.monochromeSpritePalette0[1] = _monochromePalette[ (value & 12) >> 2];
				_rcClassic.monochromeSpritePalette0[2] = _monochromePalette[ (value & 48) >> 4];
				_rcClassic.monochromeSpritePalette0[3] = _monochromePalette[ (value & 192) >> 6];

				break;

			case 0xFF49:
				// sprite palette 1 data
				_rc.ioPorts[address - 0xFF00] = value;

				_rcClassic.monochromeSpritePalette1[0] = _monochromePalette[value & 3];
				_rcClassic.monochromeSpritePalette1[1] = _monochromePalette[ (value & 12) >> 2];
				_rcClassic.monochromeSpritePalette1[2] = _monochromePalette[ (value & 48) >> 4];
				_rcClassic.monochromeSpritePalette1[3] = _monochromePalette[ (value & 192) >> 6];

				break;

			case 0xFF4A:
				// window y
				_rc.ioPorts[address - 0xFF00] = value;
				_rc.windowY = value;

				break;

			case 0xFF4B:
				// lcd y compare
				_rc.ioPorts[address - 0xFF00] = value;
				_rc.windowX = value - 7;

				break;

			case 0xFF4D:
				// prepare speed switch
				_rc.ioPorts[address - 0xFF00] = value;

				break;

			case 0xFF4F:
				// vram bank
				_rc.ioPorts[address - 0xFF00] = value;

				// TODO: make different checks, that are based on the users wishes
				if (_cartridge->GetHeader().platformSupport == PlatformSupport::GAMEBOY_COLOR_SUPPORT ||
						_cartridge->GetHeader().platformSupport != PlatformSupport::GAMEBOY_COLOR_ONLY)
				{
					_rc.selectedVideoRamBank = value & 0x01;
				}

				break;

			case 0xFF51:
				// new dma source, high
				_rc.ioPorts[address - 0xFF00] = value;

				break;

			case 0xFF52:
				// new dma source, low
				_rc.ioPorts[address - 0xFF00] = value;

				break;

			case 0xFF53:
				// new dma destination, high
				_rc.ioPorts[address - 0xFF00] = value;

				break;

			case 0xFF54:
				// new dma destination, low
				_rc.ioPorts[address - 0xFF00] = value;

				break;

			case 0xFF55:
				// new dma length/mode/start
				_rc.ioPorts[address - 0xFF00] = value;

				if (GetBit (value, 7) && !_rcColor.dmaTransferActive)
				{
					int source = JoinBytes (_rc.ioPorts[0xFF51 - 0xFF00], _rc.ioPorts[0xFF52 - 0xFF00]) & 0x000F;
					int destination = JoinBytes (_rc.ioPorts[0xFF53 - 0xFF00], _rc.ioPorts[0xFF54 - 0xFF00]) & 0xE00F;
					int length = ((value & 0x7F) + 1) * 0x10;

					_rcColor.dmaTransferActive = GBC_TRUE;
					_rcColor.dmaTransferSourceAddress = source;
					_rcColor.dmaTransferDestinationAddress = destination;
					_rcColor.dmaTransferLength = length;
					_rcColor.currentDMATransferOffset = 0x0000;

					_rc.ioPorts[address - 0xFF00] = SetBit (_rc.ioPorts[address - 0xFF00], 7, GBC_FALSE);
				}
				else if (!GetBit (value, 7) && _rcColor.dmaTransferActive)
				{
					_rcColor.dmaTransferActive = GBC_FALSE;
					_rc.ioPorts[address - 0xFF00] = SetBit (_rc.ioPorts[address - 0xFF00], 7, GBC_TRUE);
				}
				else
				{
					int source = JoinBytes (_rc.ioPorts[0xFF51 - 0xFF00], _rc.ioPorts[0xFF52 - 0xFF00]) & 0x000F;
					int destination = JoinBytes (_rc.ioPorts[0xFF53 - 0xFF00], _rc.ioPorts[0xFF54 - 0xFF00]) & 0xE00F;
					int length = ((value & 0x7F) + 1) * 0x10;

					for (int i = 0; i < length; i++)
					{
						WriteByte (destination + i, ReadByte (source + i));
					}

					// emulate bit 7 for reading, so the dma transfer "takes some time"
					// _rc.ioPorts[0xFF55 - 0xFF00] = SetBit(_rc.ioPorts[0xFF55 - 0xFF00], 7, GBC_TRUE);
					_rc.ioPorts[address - 0xFF00] = 0xFF;
				}

				break;

			case 0xFF56:
				// infrared communications port
				_rc.ioPorts[address - 0xFF00] = value;

				break;

			case 0xFF68:
				// background palette index
				_rc.ioPorts[address - 0xFF00] = value;

				if (GetBit (value, 7)) { _colorBackgroundPaletteIndexAutoIncrement = GBC_TRUE; }
				else { _colorBackgroundPaletteIndexAutoIncrement = GBC_FALSE; }

				break;

			case 0xFF69:
				// background palette data
				// RECHECK THIS!!!
				_rc.ioPorts[address - 0xFF00] = value;

				if ((((_rc.ioPorts[0xFF68 - 0xFF00] & 0x3F) % 8) % 2) == 0)
				{
					int palette = (_rc.ioPorts[0xFF68 - 0xFF00] & 0x3F) / 8;
					int color = ((_rc.ioPorts[0xFF68 - 0xFF00] & 0x3F) % 8) / 2;

					int newRed = value & 31;
					int newGreen = (_rcColor.colorBackgroundPalettes[palette][color].GetGreen() & 24) | ((value >> 5) & 7);

					_rcColor.colorBackgroundPalettes[palette][color].SetRed (newRed);
					_rcColor.colorBackgroundPalettes[palette][color].SetGreen (newGreen);
				}
				else
				{
					int palette = (_rc.ioPorts[0xFF68 - 0xFF00] & 0x3F) / 8;
					int color = ((_rc.ioPorts[0xFF68 - 0xFF00] & 0x3F) % 8) / 2;

					int newGreen = (_rcColor.colorBackgroundPalettes[palette][color].GetGreen() & 7) | ((value << 3) & 24);
					int newBlue = (value >> 2) & 31;

					_rcColor.colorBackgroundPalettes[palette][color].SetGreen (newGreen);
					_rcColor.colorBackgroundPalettes[palette][color].SetBlue (newBlue);
				}

				if (_colorBackgroundPaletteIndexAutoIncrement) { _rc.ioPorts[0xFF68 - 0xFF00]++; } // take care because of bti 7

				break;

			case 0xFF6A:
				// sprite palette index
				_rc.ioPorts[address - 0xFF00] = value;

				if (GetBit (value, 7)) { _colorSpritePaletteIndexAutoIncrement = GBC_TRUE; }
				else { _colorSpritePaletteIndexAutoIncrement = GBC_FALSE; }

				break;

			case 0xFF6B:
				// sprite palette data
				// RECHECK THIS!!!
				_rc.ioPorts[address - 0xFF00] = value;

				if ((((_rc.ioPorts[0xFF6A - 0xFF00] & 0x3F) % 8) % 2) == 0)
				{
					int palette = (_rc.ioPorts[0xFF6A - 0xFF00] & 0x3F) / 8;
					int color = ((_rc.ioPorts[0xFF6A - 0xFF00] & 0x3F) % 8) / 2;

					int newRed = value & 31;
					int newGreen = (_rcColor.colorSpritePalettes[palette][color].GetGreen() & 24) | ((value >> 5) & 7);

					_rcColor.colorSpritePalettes[palette][color].SetRed (newRed);
					_rcColor.colorSpritePalettes[palette][color].SetGreen (newGreen);
				}
				else
				{
					int palette = (_rc.ioPorts[0xFF6A - 0xFF00] & 0x3F) / 8;
					int color = ((_rc.ioPorts[0xFF6A - 0xFF00] & 0x3F) % 8) / 2;

					int newGreen = (_rcColor.colorSpritePalettes[palette][color].GetGreen() & 7) | ((value << 3) & 24);
					int newBlue = (value >> 2) & 31;

					_rcColor.colorSpritePalettes[palette][color].SetGreen (newGreen);
					_rcColor.colorSpritePalettes[palette][color].SetBlue (newBlue);
				}


			case 0xFF70:
				// wram bank
				_rc.ioPorts[address - 0xFF00] = value;

				if ((value & 0x07) == 0x00)
				{
					_rc.selectedWorkRamBank = 0x01;
				}
				else
				{
					_rc.selectedWorkRamBank = (value & 0x07);
				}

				break;

			default:
				_rc.ioPorts[address - 0xFF00] = value;
				break;
		}
	}
	else if (address <= 0xFFFE)
	{
		_rc.highRam[address - 0xFF80] = value;
	}
	else if (address == IInterruptHandler::INTERRUPT_ENABLE_ADDRESS)
	{
		// interrupt enable
		_rc.interruptEnableRegister = value;

		_rc.verticalBlankInterruptEnabled = GetBit (value, IInterruptHandler::VERTICAL_BLANK_INTERRUPT_BIT_NUMBER);
		_rc.lcdStatusInterruptEnabled = GetBit (value, IInterruptHandler::LCD_STATUS_INTERRUPT_BIT_NUMBER);
		_rc.timerInterruptEnabled = GetBit (value, IInterruptHandler::TIMER_INTERRUPT_BIT_NUMBER);
		_rc.serialInterruptEnabled = GetBit (value, IInterruptHandler::SERIAL_INTERRUPT_BIT_NUMBER);
		_rc.joypadInterruptEnabled = GetBit (value, IInterruptHandler::JOYPAD_INTERRUPT_BIT_NUMBER);
	}
	else
	{
		ERROR (std::string ("MemoryMap: Address out of range: ") + ToHex (address));
	}
}
