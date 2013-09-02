#include "GameboyColor.hpp"

GameboyColor::GameboyColor()
	: _paused(true),
	  _pendingClocks(0),
	  _lcd(NULL),
	  _joypad(NULL),
	  _directionKeysSelected(0),
	  _buttonKeysSelected(1),
	  _cartridge(NULL),
	  _forceClassicGameboy(GBC_TRUE),
	  _hybr1s80(),
	  _speedFactor(1),
	  _timerClockFrequency(1024), // or something...
	  _timerStopped(GBC_TRUE),
	  _deviderCounter(0),
	  _timerCounter(0),
	  _monochromePalette(),
	  _colorBackgroundPaletteIndexAutoIncrement(0),
	  _colorSpritePaletteIndexAutoIncrement(0),
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
	_rc.windowTileMapDisplaySelect = 0;
	_rc.windowDisplayEnabled = GBC_FALSE;
	_rc.backgroundAndWindowTileDataSelect = 0;
	_rc.backgroundTileMapDisplaySelect = 0;
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

void GameboyColor::SetLCD(ILCD &lcd)
{
	_lcd = &lcd;
}

void GameboyColor::SetJoypad(IJoypad &joypad)
{
	_joypad = &joypad;
}

void GameboyColor::SetRom(DynamicArray<int> &rom)
{
	_cartridge = Cartridge::Create(rom);
	_cartridge->LoadRamDumpFromFile();
	
	LOG("Loaded cartridge");
	
	if (_cartridge->GetHeader().platformSupport == PlatformSupport::GAMEBOY_COLOR_SUPPORT ||
	    _cartridge->GetHeader().platformSupport != PlatformSupport::GAMEBOY_COLOR_ONLY)
	{
		_renderer = new ClassicRenderer(_rc);
		
		LOG("Using Gameboy Classic rendering method");
	}
}

IInterruptHandler &GameboyColor::GetInterruptHandler()
{
	return _hybr1s80;
}

Processor &GameboyColor::GetProcessor()
{
	return _hybr1s80;
}

Renderer &GameboyColor::GetRenderer()
{
	return (*_renderer);
}

TileMap::TileMapArray2 &GameboyColor::GetTileMap(int tileMapNumber)
{
	return _rc.tileMaps[tileMapNumber].data;
}

SpriteAttribute &GameboyColor::GetSpriteAttribute(int spriteAttributeNumber)
{
	return _rc.spriteAttributes[spriteAttributeNumber];
}

void GameboyColor::Initialize()
{
	LOG("Initializing Gameboy Color emulation");
	
	_hybr1s80.SetMemoryBus(this);
	
	Reset();
	
	_monochromePalette.colors[3].red = 0x00;
	_monochromePalette.colors[3].green = 0x00;
	_monochromePalette.colors[3].blue = 0x00;
	_monochromePalette.colors[2].red = 0x08;
	_monochromePalette.colors[2].green = 0x08;
	_monochromePalette.colors[2].blue = 0x08;
	_monochromePalette.colors[1].red = 0x10;
	_monochromePalette.colors[1].green = 0x10;
	_monochromePalette.colors[1].blue = 0x10;
	_monochromePalette.colors[0].red = 0x1F;
	_monochromePalette.colors[0].green = 0x1F;
	_monochromePalette.colors[0].blue = 0x1F;
	
	std::copy(_monochromePalette.colors.begin(),
	          _monochromePalette.colors.end(),
	          _rcClassic.monochromeBackgroundPalette.colors.begin());
	
	std::copy(_monochromePalette.colors.begin(),
	          _monochromePalette.colors.end(),
	          _rcClassic.monochromeSpritePalette0.colors.begin());
	
	std::copy(_monochromePalette.colors.begin(),
	          _monochromePalette.colors.end(),
	          _rcClassic.monochromeSpritePalette1.colors.begin());
}

void GameboyColor::Finalize()
{
	LOG("Finalizing Gameboy Color emulation");
	
	_cartridge->SaveRamDumpToFile();
}

void GameboyColor::Start()
{
	_paused = false;
	
	LOG("Started emulation");
}

void GameboyColor::Pause()
{
	_paused = true;
	
	LOG("Paused emulation");
}

bool GameboyColor::IsPaused()
{
	return _paused;
}

void GameboyColor::Reset()
{
	for (Array<int, RenderContext::VIDEO_RAM_BANK_SIZE> &videoRamBank : _rc.videoRam)
	{
		std::fill(videoRamBank.begin(), videoRamBank.end(), 0x00);
	}
	
	for (Array<int, RenderContext::WORK_RAM_BANK_SIZE> &workRamBank : _rc.workRam)
	{
		std::fill(workRamBank.begin(), workRamBank.end(), 0x00);
	}
	
	std::fill(_rc.oam.begin(), _rc.oam.end(), 0x00);
	std::fill(_rc.ioPorts.begin(), _rc.ioPorts.end(), 0x00);
	std::fill(_rc.highRam.begin(), _rc.highRam.end(), 0x00);
	
	_hybr1s80.PowerUp();
}

void GameboyColor::RenderScanline()
{		
	if (_rc.lcdY < 144)
	{
		_renderer->RenderOAMSearch();
		ExecuteMachineClocks(80 * _speedFactor);
		
		_renderer->RenderTransferData();
		ExecuteMachineClocks(172 * _speedFactor);
		
		_renderer->RenderHorizontalBlank();
		ExecuteMachineClocks(204 * _speedFactor);
	}
	else
	{
		_renderer->RenderVerticalBlank();
		ExecuteMachineClocks(456 * _speedFactor);
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
		Frame renderedFrame(_rc.rawFrame);
		
		_lcd->DrawFrame(renderedFrame);
	}
	else
	{
		ERROR("Unable to draw a frame: No LCD set.");
	}
}

void GameboyColor::ExecuteMachineClocks(int clocks)
{
	_pendingClocks += clocks;
	
	while (_pendingClocks >= CLOCK_SPEED)
	{
		_pendingClocks -= CLOCK_SPEED;
		
		_hybr1s80.Execute(CLOCK_SPEED * 4);
		UpdateTimer(CLOCK_SPEED * 4);
	}
}

void GameboyColor::UpdateTimer(int ticks)
{
	if (!_timerStopped)
	{
		_timerCounter += ticks;
		
		if (_timerCounter >= _timerClockFrequency)
		{
			_rc.ioPorts[0xFF05 - 0xFF00] += _timerCounter / _timerClockFrequency;
			
			if (_rc.ioPorts[0xFF05 - 0xFF00] > 0xFF)
			{
				_rc.ioPorts[0xFF05 - 0xFF00] = _rc.ioPorts[0xFF06  - 0xFF00];
				
				_rc.interruptHandler->SignalTimerInterrupt();
			}
			
			_timerCounter %= _timerClockFrequency;
		}
	}
	
	_deviderCounter += ticks;
		
	if (_deviderCounter > 0xFF)
	{
		_rc.ioPorts[0xFF04 - 0xFF00] += _deviderCounter / 256;
		_rc.ioPorts[0xFF04 - 0xFF00] &= 0xFF;
		
		_deviderCounter %= 256;
	}
}

inline int GameboyColor::ReadByte(int address)
{
	// das geht eleganter...
	address = address & 0xFFFF;
	
	if (address <= 0x7FFF)
	{
		return _cartridge->ReadByte(address);
	}
	else if (address <= 0x9FFF)
	{
		return _rc.videoRam[_rc.selectedVideoRamBank][address - 0x8000];
	}
	else if (address <= 0xBFFF)
	{
		return _cartridge->ReadByte(address);
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
				return ((!(_directionKeysSelected ? _joypad->GetRight() : _joypad->GetButtonA())) ? 1 : 0) |
				       ((!(_directionKeysSelected ? _joypad->GetLeft() : _joypad->GetButtonB())) ? 2 : 0) |
				       ((!(_directionKeysSelected ? _joypad->GetUp() : _joypad->GetSelect())) ? 4 : 0) |
				       ((!(_directionKeysSelected ? _joypad->GetDown() : _joypad->GetStart())) ? 8 : 0) |
				       ((!_directionKeysSelected) ? 16 : 0) |
				       ((!_buttonKeysSelected) ? 32 : 0);
			
			case 0xFF41:
				// lcd status
				return (_rc.ioPorts[0xFF41 - 0xFF00] & 0xF8) |
				       (GetEnumValue(_rc.lcdMode) & 0x03) |
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
	
	LOG(std::string("MemoryMap: Address out of range: ") + ToHex(address));
	
	return 0x00;
}

inline void GameboyColor::WriteByte(int address, int value)
{
	// das geht eleganter...
	address &= 0xFFFF;
	value &= 0xFF;
	
	if (address >= 0x0000 && address <= 0x7FFF)
	{
		_cartridge->WriteByte(address, value);
	}
	else if (address <= 0x9FFF)
	{
		//if (_rc.lcdMode != LCDMode::TRANSFERRING_DATA)
		{
			_rc.videoRam[_rc.selectedVideoRamBank][address - 0x8000] = value;
			
			if (address <= 0x97FF)
			{
				int tileRamPartAddress = (address - 0x8000) & 0xFFF0;
				int tileNumber = tileRamPartAddress >> 4;
				int y = (address % 0x10) / 2;
				
				Tile &tileToChange = _rc.tiles[_rc.selectedVideoRamBank][tileNumber];
				
				int colorNumbersLow = _rc.videoRam[_rc.selectedVideoRamBank][tileRamPartAddress + y * 2];
				int colorNumbersHigh = _rc.videoRam[_rc.selectedVideoRamBank][tileRamPartAddress + y * 2 + 1];
				
				switch (address % 2)
				{
					case 0:
						for (int x = 0; x < Tile::WIDTH; x++)
						{
							tileToChange.data[x][y] &= 2;
							tileToChange.data[x][y] |= (colorNumbersLow >> (7 - x)) & 1;
						}
						
						break;
					
					case 1:
						for (int x = 0; x < Tile::WIDTH; x++)
						{
							tileToChange.data[x][y] &= 1;
							tileToChange.data[x][y] |= ((colorNumbersHigh << 1) >> (7 - x)) & 2;
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
					
					TileMap &tileMap = _rc.tileMaps[mapNumber];
					
					tileMap.data[mapElementX][mapElementY] = _rc.videoRam[0][(mapNumber == 0) ?
																			 (0x9800 - 0x8000 + mapElementNumber) :
																			 (0x9C00 - 0x8000 + mapElementNumber)];
				}
				else
				{
					int mapNumber = (address <= 0x9BFF) ? 0 : 1;
					int mapElementNumber = (address <= 0x9BFF) ? (address - 0x9800) : (address - 0x9C00);
					
					int tileMapAttribute = _rc.videoRam[1][(mapNumber == 0) ?
										                   (0x9800 + mapElementNumber) :
										                   (0x9C00 + mapElementNumber)];
					
					TileMapAttribute &tileMapAttributeToChange = _rcColor.tileMapAttributes[mapNumber][mapElementNumber];
					
					tileMapAttributeToChange.backgroundColorPaletteNumber = tileMapAttribute & 0x07;
					tileMapAttributeToChange.tileVideoRamBankNumber = (tileMapAttribute >> 3) & 0x01;
					tileMapAttributeToChange.horizontalFlip = HorizontalFlip((tileMapAttribute >> 5) & 0x01);
					tileMapAttributeToChange.verticalFlip = VerticalFlip((tileMapAttribute >> 6) & 0x01);
					tileMapAttributeToChange.backgroundToOAMPriority = BackgroundToOAMPriority((tileMapAttribute >> 7) & 0x01);
				}
			}
		}
	}
	else if (address <= 0xBFFF)
	{
		_cartridge->WriteByte(address, value);
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
			int spriteAttributeFlags = _rc.oam[oamAddress + 3];
			
			SpriteAttribute &spriteAttributeToChange = _rc.spriteAttributes[oamAddress >> 2];
			
			switch (address % 4)
			{
				case 0:
					spriteAttributeToChange.y = _rc.oam[oamAddress] - 16;
					break;
				
				case 1:
					spriteAttributeToChange.x = _rc.oam[oamAddress + 1] - 8;
					break;
				
				case 2:
					spriteAttributeToChange.tileNumber = _rc.oam[oamAddress + 2];
					break;
				
				case 3:
					spriteAttributeToChange.colorPaletteNumber = spriteAttributeFlags & 0x07;
					spriteAttributeToChange.tileVideoRamBankNumber = (spriteAttributeFlags >> 3) & 0x01;
					spriteAttributeToChange.monochromePaletteNumber = (spriteAttributeFlags >> 4) & 0x01;
					spriteAttributeToChange.horizontalFlip = HorizontalFlip((spriteAttributeFlags >> 5) & 0x01);
					spriteAttributeToChange.verticalFlip = VerticalFlip((spriteAttributeFlags >> 6) & 0x01);
					spriteAttributeToChange.spriteToBackgroundPriority = SpriteToBackgroundPriority((spriteAttributeFlags >> 7) & 0x01);
					break;
				default:
					break;
			}
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
				
				_directionKeysSelected = !GetBit(value, 4);
				_buttonKeysSelected = !GetBit(value, 5);
				
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
				_rc.ioPorts[address - 0xFF00] = value;
				
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
				
				switch (value & 0x03)
				{
					case 0x00: _timerClockFrequency = 1024; break;
					case 0x01: _timerClockFrequency = 16; break;
					case 0x02: _timerClockFrequency = 64; break;
					case 0x03: _timerClockFrequency = 256; break;
					default: break;
				}
				
				_timerStopped = !GetBit(value, 2);
				
				break;
			
			case IInterruptHandler::INTERRUPT_REQUEST_ADDRESS:
				// interrupt request flags
				_rc.ioPorts[IInterruptHandler::INTERRUPT_REQUEST_ADDRESS - 0xFF00] = value;
				
				break;
			
			case 0xFF40:
				// lcd control
				_rc.ioPorts[address - 0xFF00] = value;
				
				_rc.backgroundDisplayEnabled = GetBit(value, 0);
				_rc.spriteDisplayEnabled = GetBit(value, 1);
				_rc.spriteSize = GetBit(value, 2);
				_rc.backgroundTileMapDisplaySelect = GetBit(value, 3);
				_rc.backgroundAndWindowTileDataSelect = GetBit(value, 4);
				_rc.windowDisplayEnabled = GetBit(value, 5);
				_rc.windowTileMapDisplaySelect = GetBit(value, 6);
				_rc.lcdDisplayEnabled = GetBit(value, 7);
				
				break;
			
			case 0xFF41:
				// lcd status register
				// doesn't have any effect
				_rc.ioPorts[address - 0xFF00] = value;
				
				//_rc.lcdMode = LCDMode(value & 0x03);
				//_rc.coincidenceFlag = GetBit(value, 2);
				_rc.horizontalBlankInterruptEnabled = GetBit(value, 3);
				_rc.verticalBlankInterruptEnabledInLCD = GetBit(value, 4);
				_rc.oamInterruptEnabled = GetBit(value, 5);
				_rc.coincidenceInterruptEnabled = GetBit(value, 6);
				
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
					WriteByte(0xFE00 + i, ReadByte((value << 8) + i));
				}
				
				break;
			
			case 0xFF47:
				// background palette data
				_rc.ioPorts[address - 0xFF00] = value;
				
				_rcClassic.monochromeBackgroundPalette.colors[0] = _monochromePalette.colors[value & 3];
				_rcClassic.monochromeBackgroundPalette.colors[1] = _monochromePalette.colors[(value & 12) >> 2];
				_rcClassic.monochromeBackgroundPalette.colors[2] = _monochromePalette.colors[(value & 48) >> 4];
				_rcClassic.monochromeBackgroundPalette.colors[3] = _monochromePalette.colors[(value & 192) >> 6];
				
				break;
			
			case 0xFF48:
				// sprite palette 0 data
				_rc.ioPorts[address - 0xFF00] = value;
				
				_rcClassic.monochromeSpritePalette0.colors[0] = _monochromePalette.colors[value & 3];
				_rcClassic.monochromeSpritePalette0.colors[1] = _monochromePalette.colors[(value & 12) >> 2];
				_rcClassic.monochromeSpritePalette0.colors[2] = _monochromePalette.colors[(value & 48) >> 4];
				_rcClassic.monochromeSpritePalette0.colors[3] = _monochromePalette.colors[(value & 192) >> 6];
				
				break;
			
			case 0xFF49:
				// sprite palette 1 data
				_rc.ioPorts[address - 0xFF00] = value;
				
				_rcClassic.monochromeSpritePalette1.colors[0] = _monochromePalette.colors[value & 3];
				_rcClassic.monochromeSpritePalette1.colors[1] = _monochromePalette.colors[(value & 12) >> 2];
				_rcClassic.monochromeSpritePalette1.colors[2] = _monochromePalette.colors[(value & 48) >> 4];
				_rcClassic.monochromeSpritePalette1.colors[3] = _monochromePalette.colors[(value & 192) >> 6];
				
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
				
				if (GetBit(value, 7) && !_rcColor.dmaTransferActive)
				{
					int source = JoinBytes(_rc.ioPorts[0xFF51 - 0xFF00], _rc.ioPorts[0xFF52 - 0xFF00]) & 0x000F;
					int destination = JoinBytes(_rc.ioPorts[0xFF53 - 0xFF00], _rc.ioPorts[0xFF54 - 0xFF00]) & 0xE00F;
					int length = ((value & 0x7F) + 1) * 0x10;
				
					_rcColor.dmaTransferActive = GBC_TRUE;
					_rcColor.dmaTransferSourceAddress = source;
					_rcColor.dmaTransferDestinationAddress = destination;
					_rcColor.dmaTransferLength = length;
					_rcColor.currentDMATransferOffset = 0x0000;
					
					_rc.ioPorts[address - 0xFF00] = SetBit(_rc.ioPorts[address - 0xFF00], 7, GBC_FALSE);
				}
				else if (!GetBit(value, 7) && _rcColor.dmaTransferActive)
				{
					_rcColor.dmaTransferActive = GBC_FALSE;
					_rc.ioPorts[address - 0xFF00] = SetBit(_rc.ioPorts[address - 0xFF00], 7, GBC_TRUE);
				}
				else
				{
					int source = JoinBytes(_rc.ioPorts[0xFF51 - 0xFF00], _rc.ioPorts[0xFF52 - 0xFF00]) & 0x000F;
					int destination = JoinBytes(_rc.ioPorts[0xFF53 - 0xFF00], _rc.ioPorts[0xFF54 - 0xFF00]) & 0xE00F;
					int length = ((value & 0x7F) + 1) * 0x10;
					
					for (int i = 0; i < length; i++)
					{
						WriteByte(destination + i, ReadByte(source + i));
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
				
				if (GetBit(value, 7)) _colorBackgroundPaletteIndexAutoIncrement = GBC_TRUE;
				else _colorBackgroundPaletteIndexAutoIncrement = GBC_FALSE;
				
				break;
			
			case 0xFF69:
				// background palette data
				// RECHECK THIS!!!
				_rc.ioPorts[address - 0xFF00] = value;
				
				if ((((_rc.ioPorts[0xFF68 - 0xFF00] & 0x3F) % 8) % 2) == 0)
				{
					int palette = (_rc.ioPorts[0xFF68 - 0xFF00] & 0x3F) / 8;
					int color = ((_rc.ioPorts[0xFF68 - 0xFF00] & 0x3F) % 8) / 2;
					
					_rcColor.colorBackgroundPalettes[palette].colors[color].red = value & 31;
					_rcColor.colorBackgroundPalettes[palette].colors[color].green &= 24;
					_rcColor.colorBackgroundPalettes[palette].colors[color].green |= (value >> 5) & 7;
				}
				else
				{
					int palette = (_rc.ioPorts[0xFF68 - 0xFF00] & 0x3F) / 8;
					int color = ((_rc.ioPorts[0xFF68 - 0xFF00] & 0x3F) % 8) / 2;
					
					_rcColor.colorBackgroundPalettes[palette].colors[color].green &= 7;
					_rcColor.colorBackgroundPalettes[palette].colors[color].green |= (value << 3) & 24;
					_rcColor.colorBackgroundPalettes[palette].colors[color].blue = (value >> 2) & 31;
				}
				
				if (_colorBackgroundPaletteIndexAutoIncrement) _rc.ioPorts[0xFF68 - 0xFF00]++; // take care because of bti 7
				
				break;
			
			case 0xFF6A:
				// sprite palette index
				_rc.ioPorts[address - 0xFF00] = value;
				
				if (GetBit(value, 7)) _colorSpritePaletteIndexAutoIncrement = GBC_TRUE;
				else _colorSpritePaletteIndexAutoIncrement = GBC_FALSE;
				
				break;
			
			case 0xFF6B:
				// sprite palette data
				// RECHECK THIS!!!
				_rc.ioPorts[address - 0xFF00] = value;
				
				if ((((_rc.ioPorts[0xFF6A - 0xFF00] & 0x3F) % 8) % 2) == 0)
				{
					int palette = (_rc.ioPorts[0xFF6A - 0xFF00] & 0x3F) / 8;
					int color = ((_rc.ioPorts[0xFF6A - 0xFF00] & 0x3F) % 8) / 2;
					
					_rcColor.colorSpritePalettes[palette].colors[color].red = value & 31;
					_rcColor.colorSpritePalettes[palette].colors[color].green &= 24;
					_rcColor.colorSpritePalettes[palette].colors[color].green |= (value >> 5) & 7;
				}
				else
				{
					int palette = (_rc.ioPorts[0xFF6A - 0xFF00] & 0x3F) / 8;
					int color = ((_rc.ioPorts[0xFF6A - 0xFF00] & 0x3F) % 8) / 2;
					
					_rcColor.colorSpritePalettes[palette].colors[color].green &= 7;
					_rcColor.colorSpritePalettes[palette].colors[color].green |= (value << 3) & 24;
					_rcColor.colorSpritePalettes[palette].colors[color].blue = (value >> 2) & 31;
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
			
			default: _rc.ioPorts[address - 0xFF00] = value; break;
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
		
		_rc.verticalBlankInterruptEnabled = GetBit(value, IInterruptHandler::VERTICAL_BLANK_INTERRUPT_BIT_NUMBER);
		_rc.lcdStatusInterruptEnabled = GetBit(value, IInterruptHandler::LCD_STATUS_INTERRUPT_BIT_NUMBER);
		_rc.timerInterruptEnabled = GetBit(value, IInterruptHandler::TIMER_INTERRUPT_BIT_NUMBER);
		_rc.serialInterruptEnabled = GetBit(value, IInterruptHandler::SERIAL_INTERRUPT_BIT_NUMBER);
		_rc.joypadInterruptEnabled = GetBit(value, IInterruptHandler::JOYPAD_INTERRUPT_BIT_NUMBER);
	}
	else
	{
		ERROR(std::string("MemoryMap: Address out of range: ") + ToHex(address));
	}
}
