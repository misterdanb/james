#include "GameboyColor.hpp"

using namespace gbc;
using namespace gbc::core;
using namespace cpu;
using namespace cartridges;

GameboyColor::GameboyColor()
	: _joypad(NULL),
	  _directionKeysSelected(0),
	  _buttonKeysSelected(1),
	  _lcd(NULL),
	  _cartridge(NULL),
	  _forceClassicGameboy(GBC_TRUE),
	  _hybr1s80(),
	  _speedFactor(1),
	  _timerClockFrequency(1024), // or something...
	  _timerStopped(GBC_TRUE),
	  _deviderCounter(0),
	  _timerCounter(0),
	  _colorBackgroundPaletteIndexAutoIncrement(0),
	  _colorSpritePaletteIndexAutoIncrement(0)
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
	delete _cartridge;
	delete _renderer;
}

void GameboyColor::SetLCD(ILCD *lcd)
{
	_lcd = lcd;
}

void GameboyColor::SetJoypad(IJoypad *joypad)
{
	_joypad = joypad;
}

void GameboyColor::SetRom(DynamicArray<int> &rom)
{
	LOG_L2("Loading cartridge");
	
	_cartridge = cartridges::Cartridge::Create(rom);
	
	if (_cartridge->GetHeader().platformSupport == PlatformSupport::GAMEBOY_COLOR_SUPPORT ||
	    _cartridge->GetHeader().platformSupport != PlatformSupport::GAMEBOY_COLOR_ONLY)
	{
		_renderer = new ClassicRenderer(_rc);
	}
}

IInterruptHandler &GameboyColor::GetInterruptHandler()
{
	return _hybr1s80;
}

Renderer &GameboyColor::GetRenderer()
{
	return (*_renderer);
}

void GameboyColor::Initialize()
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

void GameboyColor::RenderScanline()
{
	if (_rc.lcdY < 144)
	{
		//DoOAMSearch();
		_renderer->RenderOAMSearch();
		ExecuteMachineClocks(80 * _speedFactor);
		
		//DoTransferData();
		_renderer->RenderTransferData();
		ExecuteMachineClocks(172 * _speedFactor);
		
		//DoHBlank();
		_renderer->RenderHorizontalBlank();
		ExecuteMachineClocks(204 * _speedFactor);
	}
	else
	{
		//DoVBlank();
		_renderer->RenderVerticalBlank();
		ExecuteMachineClocks(456 * _speedFactor);
	}
	
	if (_lcd)
	{
		_lcd->DrawFrame(Frame(_rc.rawFrame));
	}
	else
	{
		ERROR("GameboyColor: No LCD set.");
	}
}

void GameboyColor::RenderFrame()
{
	for (int i = 0; i < 154; i++)
	{	
		RenderScanline();
	}
}

void GameboyColor::ExecuteMachineClocks(int clocks)
{
	for (int i = 0; i < clocks * 4; i++)
	{
		// make a cpu step
		_hybr1s80.Step();
		
		// make a timer step
		if (_deviderCounter >= 256)
		{
			_rc.ioPorts[0xFF04 - 0xFF00]++;
			_deviderCounter = 0;
		}
		else
		{
			_deviderCounter++;
		}
		
		if (GetBit(_rc.ioPorts[0xFF07 - 0xFF00], 2))
		{
			if (_timerCounter >= _timerClockFrequency)
			{
				if (_rc.ioPorts[0xFF05 - 0xFF00] + 1 >= 0xFF)
				{
					_rc.ioPorts[0xFF05 - 0xFF00] = _rc.ioPorts[0xFF06  - 0xFF00];
					
					_rc.interruptHandler->SignalTimerInterrupt();
				}
				else
				{
					_rc.ioPorts[0xFF05 - 0xFF00]++;
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

int GameboyColor::ReadByte(int address)
{
	// das geht eleganter...
	address &= 0xFFFF;
	
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
		}
		
		return _rc.ioPorts[address - 0xFF00];
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

void GameboyColor::WriteByte(int address, int value)
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
		if (address <= 0x97FF)
		{
			int *changedTile = new int[2];
			
			changedTile[0] = _rc.selectedVideoRamBank;
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
			if (_rc.selectedVideoRamBank == 0)
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
		
		_rc.videoRam[_rc.selectedVideoRamBank][address - 0x8000] = value;
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
		
		_rc.oam[address - 0xFE00] = value;
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
				}
				
				_timerStopped = GetBit(value, 2);
				
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
				
				_rcClassic.monochromeBackgroundPalette.colors[0] = _monochromePalette.colors[value & 0b00000011];
				_rcClassic.monochromeBackgroundPalette.colors[1] = _monochromePalette.colors[(value & 0b00001100) >> 2];
				_rcClassic.monochromeBackgroundPalette.colors[2] = _monochromePalette.colors[(value & 0b00110000) >> 4];
				_rcClassic.monochromeBackgroundPalette.colors[3] = _monochromePalette.colors[(value & 0b11000000) >> 6];
				
				break;
			
			case 0xFF48:
				// sprite palette 0 data
				_rc.ioPorts[address - 0xFF00] = value;
				
				_rcClassic.monochromeSpritePalette0.colors[0] = _monochromePalette.colors[value & 0b00000011];
				_rcClassic.monochromeSpritePalette0.colors[1] = _monochromePalette.colors[(value & 0b00001100) >> 2];
				_rcClassic.monochromeSpritePalette0.colors[2] = _monochromePalette.colors[(value & 0b00110000) >> 4];
				_rcClassic.monochromeSpritePalette0.colors[3] = _monochromePalette.colors[(value & 0b11000000) >> 6];
				
				break;
			
			case 0xFF49:
				// sprite palette 1 data
				_rc.ioPorts[address - 0xFF00] = value;
				
				_rcClassic.monochromeSpritePalette1.colors[0] = _monochromePalette.colors[value & 0b00000011];
				_rcClassic.monochromeSpritePalette1.colors[1] = _monochromePalette.colors[(value & 0b00001100) >> 2];
				_rcClassic.monochromeSpritePalette1.colors[2] = _monochromePalette.colors[(value & 0b00110000) >> 4];
				_rcClassic.monochromeSpritePalette1.colors[3] = _monochromePalette.colors[(value & 0b11000000) >> 6];
				
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
				_rc.selectedVideoRamBank = value & 0x01;
				
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
					
					_rcColor.colorBackgroundPalettes[palette].colors[color].red = value & 0b00011111;
					_rcColor.colorBackgroundPalettes[palette].colors[color].green &= 0b00011000;
					_rcColor.colorBackgroundPalettes[palette].colors[color].green |= (value >> 5) & 0b00000111;
				}
				else
				{
					int palette = (_rc.ioPorts[0xFF68 - 0xFF00] & 0x3F) / 8;
					int color = ((_rc.ioPorts[0xFF68 - 0xFF00] & 0x3F) % 8) / 2;
					
					_rcColor.colorBackgroundPalettes[palette].colors[color].green &= 0b00000111;
					_rcColor.colorBackgroundPalettes[palette].colors[color].green |= (value << 3) & 0b00011000;
					_rcColor.colorBackgroundPalettes[palette].colors[color].blue = (value >> 2) & 0b00011111;
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
					
					_rcColor.colorSpritePalettes[palette].colors[color].red = value & 0b00011111;
					_rcColor.colorSpritePalettes[palette].colors[color].green &= 0b00011000;
					_rcColor.colorSpritePalettes[palette].colors[color].green |= (value >> 5) & 0b00000111;
				}
				else
				{
					int palette = (_rc.ioPorts[0xFF6A - 0xFF00] & 0x3F) / 8;
					int color = ((_rc.ioPorts[0xFF6A - 0xFF00] & 0x3F) % 8) / 2;
					
					_rcColor.colorSpritePalettes[palette].colors[color].green &= 0b00000111;
					_rcColor.colorSpritePalettes[palette].colors[color].green |= (value << 3) & 0b00011000;
					_rcColor.colorSpritePalettes[palette].colors[color].blue = (value >> 2) & 0b00011111;
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
