#include "GameboyColor.hpp"

using namespace gbc::core::cpu;
using namespace gbc::core::cartridges;

gbc::core::GameboyColor::GameboyColor()
	: _joypad(NULL),
	  _directionKeysSelected(0),
	  _buttonKeysSelected(1),
	  _lcd(NULL),
	  _cartridge(NULL),
	  _forceClassicGameboy(GBC_TRUE),
	  _hybr1s80(),
	  _speedFactor(1),
	  _interruptEnableRegister(0x00),
	  _selectedWorkRamBank(1), // 0xD000 is 1-7
	  _selectedVideoRamBank(0),
	  _timerClockFrequency(1024), // or something...
	  _timerStopped(GBC_TRUE),
	  _deviderCounter(0),
	  _timerCounter(0),
	  _colorBackgroundPaletteIndexAutoIncrement(0),
	  _colorSpritePaletteIndexAutoIncrement(0)
{
	std::memset(_videoRam, 0x00, VIDEO_RAM_BANKS * VIDEO_RAM_BANK_SIZE);
	std::memset(_workRam, 0x00, WORK_RAM_BANKS * WORK_RAM_BANK_SIZE);
	std::memset(_oam, 0x00, OAM_SIZE);
	std::memset(_ioPorts, 0x00, IO_PORTS_SIZE);
	std::memset(_highRam, 0x00, HIGH_RAM_SIZE);
	
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
	_rcColor.horizontalBlankDMATransferActive = GBC_FALSE;
	_rcColor.horizontalBlankDMATransferSourceAddress = 0x0000;
	_rcColor.horizontalBlankDMATransferDestinationAddress = 0x0000;
	_rcColor.horizontalBlankDMATransferLength = 0;
	_rcColor.currentHorizontalBlankDMATransferAddressOffset = 0x0000;
}

gbc::core::GameboyColor::~GameboyColor()
{
	delete _cartridge;
}

void gbc::core::GameboyColor::SetLCD(ILCD *lcd)
{
	_lcd = lcd;
}

void gbc::core::GameboyColor::SetJoypad(IJoypad *joypad)
{
	_joypad = joypad;
}

void gbc::core::GameboyColor::SetRom(int rom[], int size)
{
	LOG_L2("Loading cartridge");
	
	_cartridge = cartridges::Cartridge::Create(rom, size);
}

gbc::core::IInterruptHandler *gbc::core::GameboyColor::GetInterruptHandler()
{
	return &_hybr1s80;
}

void gbc::core::GameboyColor::Initialize()
{
	LOG_L2("Initializing Gameboy Color emulation");
	
	_hybr1s80.SetMemoryBus(this);
	_hybr1s80.PowerUp();
	
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
	
	std::memcpy(_rcClassic.monochromeBackgroundPalette.colors, _monochromePalette.colors, sizeof(ColorPalette));
	std::memcpy(_rcClassic.monochromeSpritePalette0.colors, _monochromePalette.colors, sizeof(ColorPalette));
	std::memcpy(_rcClassic.monochromeSpritePalette1.colors, _monochromePalette.colors, sizeof(ColorPalette));
}

void gbc::core::GameboyColor::RenderScanline()
{
	if (_rc.lcdY < 144)
	{
		DoOAMSearch();
		ExecuteMachineClocks(80 * _speedFactor);
		
		DoTransferData();
		ExecuteMachineClocks(172 * _speedFactor);
		
		DoHBlank();
		ExecuteMachineClocks(204 * _speedFactor);
	}
	else
	{
		DoVBlank();
		ExecuteMachineClocks(456 * _speedFactor);
	}
}

void gbc::core::GameboyColor::RenderFrame()
{
	for (int i = 0; i < 154; i++)
	{	
		RenderScanline();
	}
}

void gbc::core::GameboyColor::ExecuteMachineClocks(int clocks)
{
	for (int i = 0; i < clocks * 4; i++)
	{
		// make a cpu step
		_hybr1s80.Step();
		
		// make a timer step
		if (_deviderCounter >= 256)
		{
			_ioPorts[0xFF04 - 0xFF00]++;
			_deviderCounter = 0;
		}
		else
		{
			_deviderCounter++;
		}
		
		if (GetBit(_ioPorts[0xFF07 - 0xFF00], 2))
		{
			if (_timerCounter >= _timerClockFrequency)
			{
				if (_ioPorts[0xFF05 - 0xFF00] + 1 >= 0xFF)
				{
					_ioPorts[0xFF05 - 0xFF00] = _ioPorts[0xFF06  - 0xFF00];
					
					_hybr1s80.SignalTimerInterrupt();
				}
				else
				{
					_ioPorts[0xFF05 - 0xFF00]++;
				}
				
				_timerCounter = 0;
			}
			else
			{
				_timerCounter++;
			}
		}
	}
}

int gbc::core::GameboyColor::ReadByte(int address)
{
	// das geht eleganter...
	address &= 0xFFFF;
	
	if (address >= 0x0000 && address <= 0x7FFF)
	{
		return _cartridge->ReadByte(address);
	}
	else if (address >= 0x8000 && address <= 0x9FFF)
	{
		return _videoRam[_selectedVideoRamBank][address - 0x8000];
	}
	else if (address >= 0xA000 && address <= 0xBFFF)
	{
		return _cartridge->ReadByte(address);
	}
	else if (address >= 0xC000 && address <= 0xCFFF)
	{
		return _workRam[0][address - 0xC000];
	}
	else if (address >= 0xD000 && address <= 0xDFFF)
	{
		return _workRam[_selectedWorkRamBank][address - 0xD000];
	}
	else if (address >= 0xE000 && address <= 0xEFFF)
	{
		return _workRam[0][address - 0xE000];
	}
	else if (address >= 0xF000 && address <= 0xFDFF)
	{
		return _workRam[_selectedWorkRamBank][address - 0xF000];
	}
	else if (address >= 0xFE00 && address <= 0xFE9F)
	{
		return _oam[address - 0xFE00];
	}
	else if (address >= 0xFEA0 && address <= 0xFEFF)
	{
		return 0x00;
	}
	else if (address >= 0xFF00 && address <= 0xFF7F)
	{
		switch (address)
		{
			case 0xFF00:
				// joypad
				return ((!(_directionKeysSelected ? _joypad->GetRight() : _joypad->GetButtonA())) ? 0b00000001 : 0b00000000) |
				       ((!(_directionKeysSelected ? _joypad->GetLeft() : _joypad->GetButtonB())) ? 0b00000010 : 0b00000000) |
				       ((!(_directionKeysSelected ? _joypad->GetUp() : _joypad->GetSelect())) ? 0b00000100 : 0b00000000) |
				       ((!(_directionKeysSelected ? _joypad->GetDown() : _joypad->GetStart())) ? 0b00001000 : 0b00000000) |
				       ((!_directionKeysSelected) ? 0b00010000 : 0b00000000) |
				       ((!_buttonKeysSelected) ? 0b00100000 : 0b00000000);
			
			case 0xFF41:
				// lcd status
				return (_ioPorts[0xFF41 - 0xFF00] & 0xF8) |
				       (GetEnumValue(_rc.lcdMode) & 0x03) |
				       (_rc.coincidenceFlag ? 0x04 : 0x00);
			
			case 0xFF44:
				// lcd y
				return _rc.lcdY;
			
			case 0xFF4F:
				// vram bank
				return _selectedVideoRamBank;
			
			case 0xFF70:
				// wram bank
				return _selectedWorkRamBank;
		}
		
		return _ioPorts[address - 0xFF00];
	}
	else if (address >= 0xFF80 && address <= 0xFFFE)
	{
		return _highRam[address - 0xFF80];
	}
	else if (address == 0xFFFF)
	{
		// interrupt enable
		return _interruptEnableRegister;
	}
	
	LOG(std::string("MemoryMap: Address out of range: ") + ToHex(address));
	
	return 0x00;
}

void gbc::core::GameboyColor::WriteByte(int address, int value)
{
	// das geht eleganter...
	address &= 0xFFFF;
	value &= 0xFF;
	
	if (address >= 0x0000 && address <= 0x7FFF)
	{
		_cartridge->WriteByte(address, value);
	}
	else if (address >= 0x8000 && address <= 0x9FFF)
	{
		if (address <= 0x97FF)
		{
			int *changedTile = new int[2];
			
			changedTile[0] = _selectedVideoRamBank;
			changedTile[1] = ((address - 0x8000) >> 4) & 0x0FFF;
			
			int isAlreadyInList = GBC_FALSE;
				
			for (int i = 0; i < _rc.changedTiles.size(); i++)
			{
				if (changedTile[0] == _rc.changedTiles[i][0] &&
				    changedTile[1] == _rc.changedTiles[i][1])
				{
					isAlreadyInList = GBC_TRUE;
					break;
				}
			}
			
			if (!isAlreadyInList)
			{
				_rc.changedTiles.push_back(changedTile);
			}
		}
		else
		{
			if (_selectedVideoRamBank == 0)
			{
				int *changedBackgroundMapElement = new int[2];
				
				changedBackgroundMapElement[0] = (address <= 0x9BFF) ? 0 : 1;
				changedBackgroundMapElement[1] = (address <= 0x9BFF) ? (address - 0x9800) : (address - 0x9C00);
				
				int isAlreadyInList = GBC_FALSE;
				
				for (int i = 0; i < _rc.changedTileMapElements.size(); i++)
				{
					if (changedBackgroundMapElement[0] == _rc.changedTileMapElements[i][0] &&
					    changedBackgroundMapElement[1] == _rc.changedTileMapElements[i][1])
					{
						isAlreadyInList = GBC_TRUE;
						break;
					}
				}
				
				if (!isAlreadyInList)
				{
					_rc.changedTileMapElements.push_back(changedBackgroundMapElement);
				}
			}
			else
			{
				int *changedTileMapAttribute = new int[2];
				
				changedTileMapAttribute[0] = (address <= 0x9BFF) ? 0 : 1;
				changedTileMapAttribute[1] = (address <= 0x9BFF) ? (address - 0x9800) : (address - 0x9C00);
				
				int isAlreadyInList = GBC_FALSE;
				
				for (int i = 0; i < _rcColor.changedTileMapAttributes.size(); i++)
				{
					if (changedTileMapAttribute[0] == _rcColor.changedTileMapAttributes[i][0] &&
					    changedTileMapAttribute[1] == _rcColor.changedTileMapAttributes[i][1])
					{
						isAlreadyInList = GBC_TRUE;
						break;
					}
				}
				
				if (!isAlreadyInList)
				{
					_rcColor.changedTileMapAttributes.push_back(changedTileMapAttribute);
				}
			}
		}
		
		_videoRam[_selectedVideoRamBank][address - 0x8000] = value;
	}
	else if (address >= 0xA000 && address <= 0xBFFF)
	{
		_cartridge->WriteByte(address, value);
	}
	else if (address >= 0xC000 && address <= 0xCFFF)
	{
		_workRam[0][address - 0xC000] = value;
	}
	else if (address >= 0xD000 && address <= 0xDFFF)
	{
		_workRam[_selectedWorkRamBank][address - 0xD000] = value;
	}
	else if (address >= 0xE000 && address <= 0xEFFF)
	{
		_workRam[0][address - 0xE000] = value;
	}
	else if (address >= 0xF000 && address <= 0xFDFF)
	{
		_workRam[_selectedWorkRamBank][address - 0xF000] = value;
	}
	else if (address >= 0xFE00 && address <= 0xFE9F)
	{
		int changedSpriteAttribute = (address - 0xFE00) >> 2;
		
		int isAlreadyInList = GBC_FALSE;
		
		for (int i = 0; i < _rc.changedSpriteAttributes.size(); i++)
		{
			if (changedSpriteAttribute == _rc.changedSpriteAttributes[i])
			{
				isAlreadyInList = GBC_TRUE;
				break;
			}
		}
		
		if (!isAlreadyInList)
		{
			_rc.changedSpriteAttributes.push_back(changedSpriteAttribute);
		}
		
		_oam[address - 0xFE00] = value;
	}
	else if (address >= 0xFEA0 && address <= 0xFEFF)
	{
	}
	else if (address >= 0xFF00 && address <= 0xFF7F)
	{
		switch (address)
		{
			case 0xFF00:
				// joypad input
				_ioPorts[address - 0xFF00] = value;
				
				_directionKeysSelected = !GetBit(value, 4);
				_buttonKeysSelected = !GetBit(value, 5);
				
				break;
			
			case 0xFF01:
				// serial transfer data
				_ioPorts[address - 0xFF00] = value;
				
				break;
			
			case 0xFF02:
				// serial transfer control
				_ioPorts[address - 0xFF00] = value;
				
				break;
			
			case 0xFF04:
				// write to devider register
				_ioPorts[address - 0xFF00] = value;
				
				break;
			
			case 0xFF05:
				// timer counter
				_ioPorts[address - 0xFF00] = value;
				
				break;
			
			case 0xFF06:
				// timer modulo
				_ioPorts[address - 0xFF00] = value;
				
				break;
			
			case 0xFF07:
				// timer control
				_ioPorts[address - 0xFF00] = value;
				
				switch (value & 0x03)
				{
					case 0x00: _timerClockFrequency = 1024; break;
					case 0x01: _timerClockFrequency = 16; break;
					case 0x02: _timerClockFrequency = 64; break;
					case 0x03: _timerClockFrequency = 256; break;
				}
				
				_timerStopped = GetBit(value, 2);
				
				break;
			
			case IInterruptHandler::INTERRUPT_REQUEST_ADDRESS:
				// interrupt request flags
				_ioPorts[IInterruptHandler::INTERRUPT_REQUEST_ADDRESS - 0xFF00] = value;
				
				break;
			
			case 0xFF40:
				// lcd control
				_ioPorts[address - 0xFF00] = value;
				
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
				_ioPorts[address - 0xFF00] = value;
				
				//_rc.lcdMode = LCDMode(value & 0x03);
				//_rc.coincidenceFlag = GetBit(value, 2);
				_rc.horizontalBlankInterruptEnabled = GetBit(value, 3);
				_rc.verticalBlankInterruptEnabledInLCD = GetBit(value, 4);
				_rc.oamInterruptEnabled = GetBit(value, 5);
				_rc.coincidenceInterruptEnabled = GetBit(value, 6);
				
				break;
			
			case 0xFF42:
				// scroll y
				_ioPorts[address - 0xFF00] = value;
				_rc.scrollY = value;
				
				break;
			
			case 0xFF43:
				// scroll x
				_ioPorts[address - 0xFF00] = value;
				_rc.scrollX = value;
				
				break;
			
			case 0xFF44:
				// lcd y coordinate
				// doesn't have any effect
				_ioPorts[address - 0xFF00] = value;
				
				// reset counter
				_rc.lcdY = 0;
				
				break;
			
			case 0xFF45:
				// lcd y compare
				_ioPorts[address - 0xFF00] = value;
				_rc.lcdYCompare = value;
				
				break;
			
			case 0xFF46:
				// dma to oam transfer
				_ioPorts[address - 0xFF00] = value;
				
				for (int i = 0; i < 0x0100; i++)
				{
					WriteByte(0xFE00 + i, ReadByte((value << 8) + i));
				}
				
				break;
			
			case 0xFF47:
				// background palette data
				_ioPorts[address - 0xFF00] = value;
				
				_rcClassic.monochromeBackgroundPalette.colors[0] = _monochromePalette.colors[value & 0b00000011];
				_rcClassic.monochromeBackgroundPalette.colors[1] = _monochromePalette.colors[(value & 0b00001100) >> 2];
				_rcClassic.monochromeBackgroundPalette.colors[2] = _monochromePalette.colors[(value & 0b00110000) >> 4];
				_rcClassic.monochromeBackgroundPalette.colors[3] = _monochromePalette.colors[(value & 0b11000000) >> 6];
				
				break;
			
			case 0xFF48:
				// sprite palette 0 data
				_ioPorts[address - 0xFF00] = value;
				
				_rcClassic.monochromeSpritePalette0.colors[0] = _monochromePalette.colors[value & 0b00000011];
				_rcClassic.monochromeSpritePalette0.colors[1] = _monochromePalette.colors[(value & 0b00001100) >> 2];
				_rcClassic.monochromeSpritePalette0.colors[2] = _monochromePalette.colors[(value & 0b00110000) >> 4];
				_rcClassic.monochromeSpritePalette0.colors[3] = _monochromePalette.colors[(value & 0b11000000) >> 6];
				
				break;
			
			case 0xFF49:
				// sprite palette 1 data
				_ioPorts[address - 0xFF00] = value;
				
				_rcClassic.monochromeSpritePalette1.colors[0] = _monochromePalette.colors[value & 0b00000011];
				_rcClassic.monochromeSpritePalette1.colors[1] = _monochromePalette.colors[(value & 0b00001100) >> 2];
				_rcClassic.monochromeSpritePalette1.colors[2] = _monochromePalette.colors[(value & 0b00110000) >> 4];
				_rcClassic.monochromeSpritePalette1.colors[3] = _monochromePalette.colors[(value & 0b11000000) >> 6];
				
				break;
			
			case 0xFF4A:
				// window y
				_ioPorts[address - 0xFF00] = value;
				_rc.windowY = value;
				
				break;
			
			case 0xFF4B:
				// lcd y compare
				_ioPorts[address - 0xFF00] = value;
				_rc.windowX = value - 7;
				
				break;
			
			case 0xFF4D:
				// prepare speed switch
				_ioPorts[address - 0xFF00] = value;
				
				break;
			
			case 0xFF4F:
				// vram bank
				_ioPorts[address - 0xFF00] = value;
				_selectedVideoRamBank = value & 0x01;
				
				break;
			
			case 0xFF51:
				// new dma source, high
				_ioPorts[address - 0xFF00] = value;
				
				break;
			
			case 0xFF52:
				// new dma source, low
				_ioPorts[address - 0xFF00] = value;
				
				break;
			
			case 0xFF53:
				// new dma destination, high
				_ioPorts[address - 0xFF00] = value;
				
				break;
			
			case 0xFF54:
				// new dma destination, low
				_ioPorts[address - 0xFF00] = value;
				
				break;
			
			case 0xFF55:
				// new dma length/mode/start
				_ioPorts[address - 0xFF00] = value;
				
				if (GetBit(value, 7) && !_rcColor.horizontalBlankDMATransferActive)
				{
					int source = JoinBytes(_ioPorts[0xFF51 - 0xFF00], _ioPorts[0xFF52 - 0xFF00]) & 0x000F;
					int destination = JoinBytes(_ioPorts[0xFF53 - 0xFF00], _ioPorts[0xFF54 - 0xFF00]) & 0xE00F;
					int length = ((value & 0x7F) + 1) * 0x10;
				
					_rcColor.horizontalBlankDMATransferActive = GBC_TRUE;
					_rcColor.horizontalBlankDMATransferSourceAddress = source;
					_rcColor.horizontalBlankDMATransferDestinationAddress = destination;
					_rcColor.horizontalBlankDMATransferLength = length;
					_rcColor.currentHorizontalBlankDMATransferAddressOffset = 0x0000;
					
					_ioPorts[address - 0xFF00] = SetBit(_ioPorts[address - 0xFF00], 7, GBC_FALSE);
				}
				else if (!GetBit(value, 7) && _rcColor.horizontalBlankDMATransferActive)
				{
					_rcColor.horizontalBlankDMATransferActive = GBC_FALSE;
					_ioPorts[address - 0xFF00] = SetBit(_ioPorts[address - 0xFF00], 7, GBC_TRUE);
				}
				else
				{
					int source = JoinBytes(_ioPorts[0xFF51 - 0xFF00], _ioPorts[0xFF52 - 0xFF00]) & 0x000F;
					int destination = JoinBytes(_ioPorts[0xFF53 - 0xFF00], _ioPorts[0xFF54 - 0xFF00]) & 0xE00F;
					int length = ((value & 0x7F) + 1) * 0x10;
					
					for (int i = 0; i < length; i++)
					{
						WriteByte(destination + i, ReadByte(source + i));
					}
					
					// emulate bit 7 for reading, so the dma transfer "takes some time"
					// _ioPorts[0xFF55 - 0xFF00] = SetBit(_ioPorts[0xFF55 - 0xFF00], 7, GBC_TRUE);
					_ioPorts[address - 0xFF00] = 0xFF;
				}
				
				break;
			
			case 0xFF56:
				// infrared communications port
				_ioPorts[address - 0xFF00] = value;
				
				break;
			
			case 0xFF68:
				// background palette index
				_ioPorts[address - 0xFF00] = value;
				
				if (GetBit(value, 7)) _colorBackgroundPaletteIndexAutoIncrement = GBC_TRUE;
				else _colorBackgroundPaletteIndexAutoIncrement = GBC_FALSE;
				
				break;
			
			case 0xFF69:
				// background palette data
				// RECHECK THIS!!!
				_ioPorts[address - 0xFF00] = value;
				
				if ((((_ioPorts[0xFF68 - 0xFF00] & 0x3F) % 8) % 2) == 0)
				{
					int palette = (_ioPorts[0xFF68 - 0xFF00] & 0x3F) / 8;
					int color = ((_ioPorts[0xFF68 - 0xFF00] & 0x3F) % 8) / 2;
					
					_rcColor.colorBackgroundPalettes[palette].colors[color].red = value & 0b00011111;
					_rcColor.colorBackgroundPalettes[palette].colors[color].green &= 0b00011000;
					_rcColor.colorBackgroundPalettes[palette].colors[color].green |= (value >> 5) & 0b00000111;
				}
				else
				{
					int palette = (_ioPorts[0xFF68 - 0xFF00] & 0x3F) / 8;
					int color = ((_ioPorts[0xFF68 - 0xFF00] & 0x3F) % 8) / 2;
					
					_rcColor.colorBackgroundPalettes[palette].colors[color].green &= 0b00000111;
					_rcColor.colorBackgroundPalettes[palette].colors[color].green |= (value << 3) & 0b00011000;
					_rcColor.colorBackgroundPalettes[palette].colors[color].blue = (value >> 2) & 0b00011111;
				}
				
				if (_colorBackgroundPaletteIndexAutoIncrement) _ioPorts[0xFF68 - 0xFF00]++; // take care because of bti 7
				
				break;
			
			case 0xFF6A:
				// sprite palette index
				_ioPorts[address - 0xFF00] = value;
				
				if (GetBit(value, 7)) _colorSpritePaletteIndexAutoIncrement = GBC_TRUE;
				else _colorSpritePaletteIndexAutoIncrement = GBC_FALSE;
				
				break;
			
			case 0xFF6B:
				// sprite palette data
				// RECHECK THIS!!!
				_ioPorts[address - 0xFF00] = value;
				
				if ((((_ioPorts[0xFF6A - 0xFF00] & 0x3F) % 8) % 2) == 0)
				{
					int palette = (_ioPorts[0xFF6A - 0xFF00] & 0x3F) / 8;
					int color = ((_ioPorts[0xFF6A - 0xFF00] & 0x3F) % 8) / 2;
					
					_rcColor.colorSpritePalettes[palette].colors[color].red = value & 0b00011111;
					_rcColor.colorSpritePalettes[palette].colors[color].green &= 0b00011000;
					_rcColor.colorSpritePalettes[palette].colors[color].green |= (value >> 5) & 0b00000111;
				}
				else
				{
					int palette = (_ioPorts[0xFF6A - 0xFF00] & 0x3F) / 8;
					int color = ((_ioPorts[0xFF6A - 0xFF00] & 0x3F) % 8) / 2;
					
					_rcColor.colorSpritePalettes[palette].colors[color].green &= 0b00000111;
					_rcColor.colorSpritePalettes[palette].colors[color].green |= (value << 3) & 0b00011000;
					_rcColor.colorSpritePalettes[palette].colors[color].blue = (value >> 2) & 0b00011111;
				}
				
			
			case 0xFF70:
				// wram bank
				_ioPorts[address - 0xFF00] = value;
				
				if ((value & 0x07) == 0x00)
				{
					_selectedWorkRamBank = 0x01;
				}
				else
				{
					_selectedWorkRamBank = (value & 0x07);
				}
				
				break;
			
			default: _ioPorts[address - 0xFF00] = value; break;
		}
	}
	else if (address >= 0xFF80 && address <= 0xFFFE)
	{
		_highRam[address - 0xFF80] = value;
	}
	else if (address == IInterruptHandler::INTERRUPT_ENABLE_ADDRESS)
	{
		// interrupt enable
		_interruptEnableRegister = value;
		
		_rc.verticalBlankInterruptEnabled = GetBit(value, IInterruptHandler::VERTICAL_BLANK_INTERRUPT_ENABLE_BIT);
		_rc.lcdStatusInterruptEnabled = GetBit(value, IInterruptHandler::LCD_STATUS_INTERRUPT_ENABLE_BIT);
		_rc.timerInterruptEnabled = GetBit(value, IInterruptHandler::TIMER_INTERRUPT_ENABLE_BIT);
		_rc.serialInterruptEnabled = GetBit(value, IInterruptHandler::SERIAL_INTERRUPT_ENABLE_BIT);
		_rc.joypadInterruptEnabled = GetBit(value, IInterruptHandler::JOYPAD_INTERRUPT_ENABLE_BIT);
	}
	else
	{
		LOG(std::string("MemoryMap: Address out of range: ") + ToHex(address));
	}
}

void gbc::core::GameboyColor::DoOAMSearch()
{
	_rc.lcdMode = LCDMode::SEARCHING_OAM;
	
	_rc.coincidenceFlag = (_rc.lcdY == _rc.lcdYCompare);
	
	if (_rc.oamInterruptEnabled)
	{
		_hybr1s80.SignalLCDStatusInterrupt();
	}
	
	if (_rc.coincidenceInterruptEnabled && _rc.coincidenceFlag)
	{
		_hybr1s80.SignalLCDStatusInterrupt();
	}
	
	UpdateSpriteAttributes();
}

void gbc::core::GameboyColor::DoTransferData()
{
	_rc.lcdMode = LCDMode::TRANSFERRING_DATA;
	
	UpdateTiles();
	UpdateBackgroundMapElements();
	UpdateTileMapAttributes();
	UpdateSpriteAttributes();
	
	if (_rc.lcdDisplayEnabled)
	{
		if ((_cartridge->GetHeader().platformSupport == PlatformSupport::GAMEBOY_COLOR_SUPPORT ||
	        _cartridge->GetHeader().platformSupport == PlatformSupport::GAMEBOY_COLOR_ONLY) &&
		    !_forceClassicGameboy)
		{
			if (!_rc.backgroundDisplayEnabled)
			{
				DrawBackgroundMap(COLOR_0 | COLOR_1 | COLOR_2 | COLOR_3);
				DrawWindowMap(COLOR_0 | COLOR_1 | COLOR_2 | COLOR_3);
				DrawSprites(COLOR_0 | COLOR_1 | COLOR_2 | COLOR_3);
			}
			else
			{
				if (_rc.backgroundDisplayEnabled)
				{
					DrawBackgroundMap(COLOR_0, BackgroundToOAMPriority::USE_OAM_PRIORITY_BIT);
				}
				
				if (_rc.windowDisplayEnabled)
				{
					DrawWindowMap(COLOR_0, BackgroundToOAMPriority::USE_OAM_PRIORITY_BIT);
				}
				
				DrawSprites(COLOR_1 | COLOR_2 | COLOR_3, SpriteToBackgroundPriority::SPRITE_BEHIND_BACKGROUND);
				
				if (_rc.backgroundDisplayEnabled)
				{
					DrawBackgroundMap(COLOR_1 | COLOR_2 | COLOR_3, BackgroundToOAMPriority::USE_OAM_PRIORITY_BIT);
				}
				
				if (_rc.windowDisplayEnabled)
				{
					DrawWindowMap(COLOR_1 | COLOR_2 | COLOR_3, BackgroundToOAMPriority::USE_OAM_PRIORITY_BIT);
				}
				
				DrawSprites(COLOR_1 | COLOR_2 | COLOR_3, SpriteToBackgroundPriority::SPRITE_ABOVE_BACKGROUND);
				
				if (_rc.backgroundDisplayEnabled)
				{
					DrawBackgroundMap(COLOR_1 | COLOR_2 | COLOR_3, BackgroundToOAMPriority::BACKGROUND_PRIORITY);
				}
				
				if (_rc.windowDisplayEnabled)
				{
					DrawWindowMap(COLOR_1 | COLOR_2 | COLOR_3, BackgroundToOAMPriority::BACKGROUND_PRIORITY);
				}
			}
		}
		else
		{
			if (_rc.backgroundDisplayEnabled)
			{
				DrawBackgroundMap(COLOR_0);
			}
			
			if (_rc.windowDisplayEnabled)
			{
				DrawWindowMap(COLOR_0);
			}
			
			DrawSprites(COLOR_1 | COLOR_2 | COLOR_3, SpriteToBackgroundPriority::SPRITE_BEHIND_BACKGROUND);
			
			if (_rc.backgroundDisplayEnabled)
			{
				DrawBackgroundMap(COLOR_1 | COLOR_2 | COLOR_3);
			}
			
			if (_rc.windowDisplayEnabled)
			{
				DrawWindowMap(COLOR_1 | COLOR_2 | COLOR_3);
			}
			
			DrawSprites(COLOR_1 | COLOR_2 | COLOR_3, SpriteToBackgroundPriority::SPRITE_ABOVE_BACKGROUND);
		}
	}
}

void gbc::core::GameboyColor::DoHBlank()
{
	_rc.lcdMode = LCDMode::HORIZONTAL_BLANK;
	
	if (_rc.horizontalBlankInterruptEnabled)
	{
		_hybr1s80.SignalLCDStatusInterrupt();
	}
	
	if (_rcColor.horizontalBlankDMATransferActive)
	{
		if (_rcColor.currentHorizontalBlankDMATransferAddressOffset <
		    _rcColor.horizontalBlankDMATransferLength)
		{
			int currentSource = _rcColor.horizontalBlankDMATransferSourceAddress +
			                    _rcColor.currentHorizontalBlankDMATransferAddressOffset;
			
			int currentDestination = _rcColor.horizontalBlankDMATransferDestinationAddress +
			                         _rcColor.currentHorizontalBlankDMATransferAddressOffset;
			
			for (int i = 0; i < 0x10; i++)
			{
				WriteByte(currentDestination + i, ReadByte(currentSource + i));
			}
			
			_rcColor.currentHorizontalBlankDMATransferAddressOffset += 0x10;
			
			_ioPorts[0xFF55 - 0xFF00] &= 0x80;
			_ioPorts[0xFF55 - 0xFF00] |= (((_rcColor.horizontalBlankDMATransferLength -
			                                _rcColor.currentHorizontalBlankDMATransferAddressOffset) / 0x10) - 1) & 0x7F;
		}
		else
		{
			_ioPorts[0xFF55 - 0xFF00] = 0xFF;

			_rcColor.horizontalBlankDMATransferActive = GBC_FALSE;
			_rcColor.horizontalBlankDMATransferSourceAddress = 0x0000;
			_rcColor.horizontalBlankDMATransferDestinationAddress = 0x0000;
			_rcColor.horizontalBlankDMATransferLength = 0x0000;
			_rcColor.currentHorizontalBlankDMATransferAddressOffset = 0x0000;
		}
	}

	if (_rc.lcdY < 144)
	{
		_rc.lcdY++;
	}
}

void gbc::core::GameboyColor::DoVBlank()
{
	_rc.lcdMode = LCDMode::VERTICAL_BLANK;
	
	_rc.coincidenceFlag = (_rc.lcdY == _rc.lcdYCompare);
	
	if (_rc.coincidenceInterruptEnabled && _rc.coincidenceFlag)
	{
		_hybr1s80.SignalLCDStatusInterrupt();
	}
	
	if (_rc.verticalBlankInterruptEnabled && !_rc.verticalBlankInterruptAlreadyRequested)
	{
		_hybr1s80.SignalVBlankInterrupt();
		_rc.verticalBlankInterruptAlreadyRequested = GBC_TRUE;
	}
	
	if (_rc.lcdY >= 144)
	{
		_rc.lcdY++;
	}
	
	if (_rc.lcdY > 153)
	{
		if (_lcd)
		{
			_lcd->DrawFrame(Frame(_rawFrame));
		}
		else
		{
			ERROR("GameboyColor: No LCD set.");
		}
		
		_rc.lcdY = 0;
		_rc.verticalBlankInterruptAlreadyRequested = GBC_FALSE;
	}
}

void gbc::core::GameboyColor::UpdateTiles()
{
	int lastTile = 0;
	
	while (_rc.changedTiles.size() > 0)
	{
		int videoRamBank = _rc.changedTiles.back()[0];
		int tileNumber = _rc.changedTiles.back()[1];
		int videoRamAddress = tileNumber << 4; // * 0x10
		
		for (int y = 0; y < Tile::HEIGHT; y++)
		{
			int colorNumbersLow = _videoRam[videoRamBank][videoRamAddress + y * 2];
			int colorNumbersHigh = _videoRam[videoRamBank][videoRamAddress + y * 2 + 1];
			
			for (int x = 0; x < Tile::WIDTH; x++)
			{
				_rc.tiles[videoRamBank][tileNumber].data[x][y] = (((colorNumbersHigh << 1) >> (7 - x)) & 0b10) |
				                                                 ((colorNumbersLow >> (7 - x)) & 0b01);
			}
		}
		
		delete[] _rc.changedTiles.back();
		
		_rc.changedTiles.pop_back();
	}
}

void gbc::core::GameboyColor::UpdateBackgroundMapElements()
{
	while (_rc.changedTileMapElements.size() > 0)
	{
		int mapNumber = _rc.changedTileMapElements.back()[0];
		int mapElementNumber = _rc.changedTileMapElements.back()[1];
		
		_rc.tileMaps[mapNumber].data
		[mapElementNumber % TileMap::WIDTH]
		[mapElementNumber / TileMap::WIDTH] = _videoRam[0][(mapNumber == 0) ?
		                                                   (0x9800 - 0x8000 + mapElementNumber) :
		                                                   (0x9C00 - 0x8000 + mapElementNumber)];
		
		delete[] _rc.changedTileMapElements.back();
		
		_rc.changedTileMapElements.pop_back();
	}
}

void gbc::core::GameboyColor::UpdateTileMapAttributes()
{
	while (_rcColor.changedTileMapAttributes.size() > 0)
	{
		int mapNumber = _rcColor.changedTileMapAttributes.back()[0];
		int mapElementNumber = _rcColor.changedTileMapAttributes.back()[1];
		
		int tileMapAttribute = _videoRam[1][(mapNumber == 0) ?
		                       (0x9800 + mapElementNumber) :
		                       (0x9C00 + mapElementNumber)];
		
		TileMapAttribute &tileMapAttributeToChange = _rcColor.tileMapAttributes[mapNumber][mapElementNumber];
		
		tileMapAttributeToChange.backgroundColorPaletteNumber = tileMapAttribute & 0x07;
		tileMapAttributeToChange.tileVideoRamBankNumber = (tileMapAttribute >> 3) & 0x01;
		tileMapAttributeToChange.horizontalFlip = HorizontalFlip((tileMapAttribute >> 5) & 0x01);
		tileMapAttributeToChange.verticalFlip = VerticalFlip((tileMapAttribute >> 6) & 0x01);
		tileMapAttributeToChange.backgroundToOAMPriority = BackgroundToOAMPriority((tileMapAttribute >> 7) & 0x01);
		
		delete[] _rcColor.changedTileMapAttributes.back();
		
		_rcColor.changedTileMapAttributes.pop_back();
	}
}

void gbc::core::GameboyColor::UpdateSpriteAttributes()
{
	while (_rc.changedSpriteAttributes.size() > 0)
	{
		int spriteAttributeNumber = _rc.changedSpriteAttributes.back();
		int oamAddress = spriteAttributeNumber << 2; // * 4
		
		int spriteAttributeFlags = _oam[oamAddress + 3];
		
		SpriteAttribute &spriteAttributeToChange = _rc.spriteAttributes[spriteAttributeNumber];
		
		spriteAttributeToChange.y = _oam[oamAddress] - 16;
		spriteAttributeToChange.x = _oam[oamAddress + 1] - 8;
		spriteAttributeToChange.tileNumber = _oam[oamAddress + 2];
		
		spriteAttributeToChange.spriteColorPaletteNumber = spriteAttributeFlags & 0x07;
		spriteAttributeToChange.tileVideoRamBankNumber = (spriteAttributeFlags >> 3) & 0x01;
		spriteAttributeToChange.spriteMonochromePaletteNumber = (spriteAttributeFlags >> 4) & 0x01;
		spriteAttributeToChange.horizontalFlip = HorizontalFlip((spriteAttributeFlags >> 5) & 0x01);
		spriteAttributeToChange.verticalFlip = VerticalFlip((spriteAttributeFlags >> 6) & 0x01);
		spriteAttributeToChange.spriteToBackgroundPriority = SpriteToBackgroundPriority((spriteAttributeFlags >> 7) & 0x01);
		
		_rc.changedSpriteAttributes.pop_back();
	}
}

void gbc::core::GameboyColor::DrawSprites(int enabledColors,
                                          SpriteToBackgroundPriority spriteToBackgroundPriority)
{
	for (int i = 0; i < 40; i++)
	{
		SpriteAttribute spriteAttribute = _rc.spriteAttributes[i];
		
		if (spriteAttribute.spriteToBackgroundPriority == spriteToBackgroundPriority)
		{
			ColorPalette colorPalette = spriteAttribute.spriteMonochromePaletteNumber == 0 ?
			                            _rcClassic.monochromeSpritePalette0 :
			                            _rcClassic.monochromeSpritePalette1;
			
			if ((_cartridge->GetHeader().platformSupport == PlatformSupport::GAMEBOY_COLOR_SUPPORT ||
			    _cartridge->GetHeader().platformSupport == PlatformSupport::GAMEBOY_COLOR_ONLY) &&
			    !_forceClassicGameboy)
			{
				colorPalette = _rcColor.colorSpritePalettes[spriteAttribute.spriteColorPaletteNumber];
			}
			
			if ((_cartridge->GetHeader().platformSupport == PlatformSupport::GAMEBOY_COLOR_SUPPORT ||
			    _cartridge->GetHeader().platformSupport == PlatformSupport::GAMEBOY_COLOR_ONLY) &&
			    !_forceClassicGameboy)
			{
				DrawTile(spriteAttribute.x, spriteAttribute.y,
						 _rc.tiles[spriteAttribute.tileVideoRamBankNumber][(0x8000 - 0x8000) + spriteAttribute.tileNumber],
						 spriteAttribute.horizontalFlip,
						 spriteAttribute.verticalFlip,
						 colorPalette,
						 enabledColors);
			}
			else
			{
				DrawTile(spriteAttribute.x, spriteAttribute.y,
						 _rc.tiles[0][(0x8000 - 0x8000) + spriteAttribute.tileNumber],
						 spriteAttribute.horizontalFlip,
						 spriteAttribute.verticalFlip,
						 colorPalette,
						 enabledColors);
			}
		}
	}
}

void gbc::core::GameboyColor::DrawSprites(int enabledColors)
{
	for (int i = 0; i < 40; i++)
	{
		SpriteAttribute spriteAttribute = _rc.spriteAttributes[i];
		
		int tileNumber = (0x8000 - 0x8000) + spriteAttribute.tileNumber;
		
		ColorPalette colorPalette = spriteAttribute.spriteMonochromePaletteNumber == 0 ?
			                        _rcClassic.monochromeSpritePalette0 :
			                        _rcClassic.monochromeSpritePalette1;
		
		if ((_cartridge->GetHeader().platformSupport == PlatformSupport::GAMEBOY_COLOR_SUPPORT ||
	        _cartridge->GetHeader().platformSupport == PlatformSupport::GAMEBOY_COLOR_ONLY) &&
		    !_forceClassicGameboy)
		{
			colorPalette = _rcColor.colorSpritePalettes[spriteAttribute.spriteColorPaletteNumber];
		}			
	    
		if ((_cartridge->GetHeader().platformSupport == PlatformSupport::GAMEBOY_COLOR_SUPPORT ||
	        _cartridge->GetHeader().platformSupport == PlatformSupport::GAMEBOY_COLOR_ONLY) &&
		    !_forceClassicGameboy)
		{
			DrawTile(spriteAttribute.x, spriteAttribute.y,
					 _rc.tiles[spriteAttribute.tileVideoRamBankNumber][(0x8000 - 0x8000) + spriteAttribute.tileNumber],
					 spriteAttribute.horizontalFlip,
					 spriteAttribute.verticalFlip,
					 colorPalette,
					 enabledColors);
		}
		else
		{
			DrawTile(spriteAttribute.x, spriteAttribute.y,
					 _rc.tiles[0][(0x8000 - 0x8000) + spriteAttribute.tileNumber],
					 spriteAttribute.horizontalFlip,
					 spriteAttribute.verticalFlip,
					 colorPalette,
					 enabledColors);
		}
	}
}

void gbc::core::GameboyColor::DrawBackgroundMap(int enabledColors,
                                                BackgroundToOAMPriority backgroundToOAMPriority)
{
	for (int mapX = 0; mapX < TileMap::WIDTH; mapX++)
	{
		int mapElementX = ((mapX * Tile::WIDTH) + _rc.scrollX) / Tile::WIDTH;
		int mapElementY = (_rc.lcdY + _rc.scrollY) / Tile::HEIGHT;
		
		if (mapElementX < 0)
		{
			mapElementX += TileMap::WIDTH;
		}
		
		if (mapElementY < 0)
		{
			mapElementY += TileMap::HEIGHT;
		}
	
		mapElementX %= TileMap::WIDTH;
		mapElementY %= TileMap::HEIGHT;
		
		int mapElementNumber = (mapElementY * TileMap::WIDTH) + mapElementX;
		
		TileMapAttribute backgroundMapAttribute = _rcColor.tileMapAttributes
			                                            [_rc.backgroundTileMapDisplaySelect]
			                                            [mapElementNumber];
		
		if (backgroundMapAttribute.backgroundToOAMPriority == backgroundToOAMPriority)
		{
			DrawMapTile(mapX, -_rc.scrollX, -_rc.scrollY, 
			            _rc.backgroundTileMapDisplaySelect,
			            enabledColors,
			            _cartridge->GetHeader().platformSupport);
		}
	}
}

void gbc::core::GameboyColor::DrawBackgroundMap(int enabledColors)
{
	for (int mapX = 0; mapX < TileMap::WIDTH; mapX++)
	{
		DrawMapTile(mapX, -_rc.scrollX, -_rc.scrollY, 
		            _rc.backgroundTileMapDisplaySelect,
		            enabledColors,
		            _cartridge->GetHeader().platformSupport);
	}
}

void gbc::core::GameboyColor::DrawWindowMap(int enabledColors,
                                            BackgroundToOAMPriority backgroundToOAMPriority)
{
	for (int mapX = 0; mapX < TileMap::WIDTH; mapX++)
	{
		int mapElementX = ((mapX * Tile::WIDTH) - _rc.windowX) / Tile::WIDTH;
		int mapElementY = (_rc.lcdY - _rc.windowY) / Tile::HEIGHT;
		
		if (mapElementX < 0)
		{
			mapElementX += TileMap::WIDTH;
		}
		
		if (mapElementY < 0)
		{
			mapElementY += TileMap::HEIGHT;
		}
		
		mapElementX %= TileMap::WIDTH;
		mapElementY %= TileMap::HEIGHT;
		
		int mapElementNumber = (mapElementY * TileMap::WIDTH) + mapElementX;
		
		TileMapAttribute backgroundMapAttribute = _rcColor.tileMapAttributes
			                                            [_rc.windowTileMapDisplaySelect]
			                                            [mapElementNumber];
		
		if (backgroundMapAttribute.backgroundToOAMPriority == backgroundToOAMPriority)
		{
			DrawMapTile(mapX, _rc.windowX, _rc.windowY,
			            _rc.windowTileMapDisplaySelect,
			            enabledColors,
			            _cartridge->GetHeader().platformSupport);
		}
	}
}

void gbc::core::GameboyColor::DrawWindowMap(int enabledColors)
{
	for (int mapX = 0; mapX < TileMap::WIDTH; mapX++)
	{
		DrawMapTile(mapX, _rc.windowX, _rc.windowY,
		            _rc.windowTileMapDisplaySelect,
		            enabledColors,
		            _cartridge->GetHeader().platformSupport);
	}
}

void gbc::core::GameboyColor::DrawMapTile(int mapX,
                                          int xOffset,
                                          int yOffset,
                                          int tileMapDisplaySelect,
                                          int enabledColors,
                                          PlatformSupport platformSupport)
{
	//if (tileMapDisplaySelect == 0) return; ////////////////////
	int x = (mapX * Tile::WIDTH) + xOffset;
	int y = (_rc.lcdY - yOffset) - ((_rc.lcdY - yOffset) % Tile::HEIGHT) + yOffset;
	
	int mapElementX = ((mapX * Tile::WIDTH) - xOffset) / Tile::WIDTH;
	int mapElementY = (_rc.lcdY - yOffset) / Tile::HEIGHT;
	
	if (mapElementX < 0)
	{
		mapElementX += TileMap::WIDTH;
	}
	
	if (mapElementY < 0)
	{
		mapElementY += TileMap::HEIGHT;
	}
	
	mapElementX %= TileMap::WIDTH;
	mapElementY %= TileMap::HEIGHT;
	
	int mapElementNumber = (mapElementY * TileMap::WIDTH) + mapElementX;
	
	TileMapAttribute backgroundMapAttribute = _rcColor.tileMapAttributes
		                                      [tileMapDisplaySelect]
		                                      [mapElementNumber];
	
	int backgroundMapElement = _rc.tileMaps
		                       [tileMapDisplaySelect].data
		                       [mapElementX][mapElementY];
	
	int tileVideoRamBankNumber = 0;
	
	if ((_cartridge->GetHeader().platformSupport == PlatformSupport::GAMEBOY_COLOR_SUPPORT ||
	    _cartridge->GetHeader().platformSupport == PlatformSupport::GAMEBOY_COLOR_ONLY) &&
	    !_forceClassicGameboy)
	{
		tileVideoRamBankNumber = backgroundMapAttribute.tileVideoRamBankNumber;
	}
	
	int tileNumber = (!_rc.backgroundAndWindowTileDataSelect) ?
		             (((0x9000 - 0x8000) / 16) + GetSignedValue(backgroundMapElement)) :
		             (((0x8000 - 0x8000) / 16) + backgroundMapElement);
	
	Tile tile = _rc.tiles[tileVideoRamBankNumber][tileNumber];
	
	ColorPalette colorPalette = _rcClassic.monochromeBackgroundPalette;
	
	if ((_cartridge->GetHeader().platformSupport == PlatformSupport::GAMEBOY_COLOR_SUPPORT ||
	    _cartridge->GetHeader().platformSupport == PlatformSupport::GAMEBOY_COLOR_ONLY) &&
	    !_forceClassicGameboy)
	{
		colorPalette = _rcColor.colorBackgroundPalettes[backgroundMapAttribute.backgroundColorPaletteNumber];
	}
	
	if ((_cartridge->GetHeader().platformSupport == PlatformSupport::GAMEBOY_COLOR_SUPPORT ||
	    _cartridge->GetHeader().platformSupport == PlatformSupport::GAMEBOY_COLOR_ONLY) &&
	    !_forceClassicGameboy)
	{
		DrawTile(x, y,
			     tile,
			     backgroundMapAttribute.horizontalFlip,
			     backgroundMapAttribute.verticalFlip,
			     colorPalette,
			     enabledColors);
	}
	else
	{
		DrawTile(x, y,
			     tile,
			     HorizontalFlip::NOT_FLIPPED,
			     VerticalFlip::NOT_FLIPPED,
			     colorPalette,
			     enabledColors);
	}
}

void gbc::core::GameboyColor::DrawTile(int x, int y,
                                       Tile tile,
                                       HorizontalFlip horizontalFlip,
                                       VerticalFlip verticalFlip,
                                       ColorPalette colorPalette,
                                       int enabledColors)
{
	int tileY = _rc.lcdY - y;
	
	if (tileY >= 0 && tileY < Tile::HEIGHT)
	{
		for (int tileX = 0; tileX < Tile::WIDTH; tileX++)
		{
			int scanlinePosition = x + tileX;
			
			if (scanlinePosition >= 0 && scanlinePosition < Frame::WIDTH &&
			    _rc.lcdY >= 0 && _rc.lcdY < Frame::HEIGHT)
			{
				int realTileX = tileX;
				int realTileY = tileY;
				
				if (horizontalFlip == HorizontalFlip::FLIPPED)
				{
					realTileX = 7 - tileX;
				}
				
				if (verticalFlip == VerticalFlip::FLIPPED)
				{
					realTileY = 7 - tileY;
				}
				
				int colorNumber = tile.data[realTileX][realTileY];
				
				if (((colorNumber == 0) && (enabledColors & COLOR_0)) ||
				    ((colorNumber == 1) && (enabledColors & COLOR_1)) ||
				    ((colorNumber == 2) && (enabledColors & COLOR_2)) ||
				    ((colorNumber == 3) && (enabledColors & COLOR_3)))
				{
					_rawFrame[_rc.lcdY * Frame::WIDTH + scanlinePosition] = colorPalette.colors[colorNumber];
				}
			}
		}
	}
}
