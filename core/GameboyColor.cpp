#include "GameboyColor.hpp"

gbc::core::GameboyColor::GameboyColor()
	: _joypad(NULL),
	  _directionKeysSelected(0),
	  _buttonKeysSelected(1),
	  _lcd(NULL),
	  _cartridge(NULL),
	  _forceClassicGameboy(GBC_TRUE),
	  _hybr1s80(),
	  _speedFactor(1),
	  _videoRam({ { 0, }, { 0, } }),
	  _workRam({ { 0, }, { 0, }, { 0, }, { 0, }, { 0, }, { 0, }, { 0, }, { 0, } }),
	  _oam({ 0, }),
	  _ioPorts({ 0, }),
	  _highRam({ 0, }),
	  _interruptEnableRegister(0x00),
	  _selectedWorkRamBank(1), // 0xD000 is 1-7
	  _selectedVideoRamBank(0),
	  _timerClockFrequency(1024), // or something...
	  _timerStopped(GBC_TRUE),
	  _deviderCounter(0),
	  _timerCounter(0),
	  _vBlankInterruptAlreadyRequested(GBC_FALSE),
	  _lcdDisplayEnabled(GBC_FALSE),
	  _windowTileMapDisplaySelect(0),
	  _windowDisplayEnabled(GBC_FALSE),
	  _backgroundAndWindowTileDataSelect(0),
	  _backgroundTileMapDisplaySelect(0),
	  _spriteSize(0), // missing enum for sprite size (and implementation of this)
	  _spriteDisplayEnabled(GBC_FALSE),
	  _backgroundDisplayEnabled(GBC_FALSE),
	  _lcdMode(vBlank),
	  _coincidenceFlag(GBC_FALSE),
	  _hBlankInterruptEnabled(GBC_FALSE),
	  _vBlankInterruptEnabledInLCD(GBC_FALSE),
	  _oamInterruptEnabled(GBC_FALSE),
	  _coincidenceInterruptEnabled(GBC_FALSE),
	  _scrollX(0),
	  _scrollY(0),
	  _lcdY(0),
	  _lcdYCompare(GBC_FALSE),
	  _windowY(0),
	  _windowX(0),
	  _colorBackgroundPaletteIndexAutoIncrement(0),
	  _colorSpritePaletteIndexAutoIncrement(0),
	  _hBlankDMATransferActive(GBC_FALSE),
	  _hBlankDMATransferSourceAddress(0x0000),
	  _hBlankDMATransferDestinationAddress(0x0000),
	  _hBlankDMATransferLength(0),
	  _currentHBlankDMATransferAddressOffset(0x0000)
{
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
	
	std::memcpy(_monochromeBackgroundPalette.colors, _monochromePalette.colors, sizeof(ColorPalette));
	std::memcpy(_monochromeSpritePalette0.colors, _monochromePalette.colors, sizeof(ColorPalette));
	std::memcpy(_monochromeSpritePalette1.colors, _monochromePalette.colors, sizeof(ColorPalette));
}

void gbc::core::GameboyColor::RenderScanline()
{
	if (_lcdY < 144)
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
	
	// test output background maps
	std::cout << "Background map 0: " << std::endl;
	
	for (int y = 0; y < 32; y++)
	{
		for (int x = 0; x < 32; x++)
		{
			std::cout << ToHex(_backgroundMapElements[0][y * 32 + x]) << " ";
		}
		
		std::cout << std::endl;
	}
	
	std::cout << std::endl;
	
	std::cout << "Background map 1: " << std::endl;
	
	for (int y = 0; y < 32; y++)
	{
		for (int x = 0; x < 32; x++)
		{
			std::cout << ToHex(_backgroundMapElements[1][y * 32 + x]) << " ";
		}
		
		std::cout << std::endl;
	}
	
	std::cout << std::endl;
	
	std::cin.get();
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
		
		if (GET_BIT(_ioPorts[0xFF07 - 0xFF00], 2))
		{
			if (_timerCounter >= _timerClockFrequency)
			{
				if (_ioPorts[0xFF05 - 0xFF00] + 1 >= 0xFF)
				{
					_ioPorts[0xFF05 - 0xFF00] = _ioPorts[0xFF06  - 0xFF00];
					_ioPorts[0xFF0F - 0xFF00] = SET_BIT(_ioPorts[0xFF0F - 0xFF00], 2, GBC_TRUE);
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
		LOG_L3("Cartridge: " +
		       std::string("Read ") +
		       ToHex(_cartridge->ReadByte(address)) +
		       std::string(" from address ") +
		       ToHex(address));
		
		return _cartridge->ReadByte(address);
	}
	else if (address >= 0x8000 && address <= 0x9FFF)
	{
		LOG_L3("Video ram [" + ToDec(_selectedVideoRamBank) + "]: " +
		       std::string("Read ") +
		       ToHex(_cartridge->ReadByte(address)) +
		       std::string(" from address ") +
		       ToHex(address));
		
		return _videoRam[_selectedVideoRamBank][address - 0x8000];
	}
	else if (address >= 0xA000 && address <= 0xBFFF)
	{
		LOG_L3("Cartridge: " +
		       std::string("Read ") +
		       ToHex(_cartridge->ReadByte(address)) +
		       std::string(" from address ") +
		       ToHex(address));
		
		return _cartridge->ReadByte(address);
	}
	else if (address >= 0xC000 && address <= 0xCFFF)
	{
		LOG_L3("Work ram [0]: " +
		       std::string("Read ") +
		       ToHex(_workRam[0][address - 0xC000]) +
		       std::string(" from address ") +
		       ToHex(address));
		
		return _workRam[0][address - 0xC000];
	}
	else if (address >= 0xD000 && address <= 0xDFFF)
	{
		LOG_L3("Work ram [" + ToDec(_selectedWorkRamBank) + "]: " +
		       std::string("Read ") +
		       ToHex(_workRam[_selectedWorkRamBank][address - 0xD000]) +
		       std::string(" from address ") +
		       ToHex(address));
		
		return _workRam[_selectedWorkRamBank][address - 0xD000];
	}
	else if (address >= 0xE000 && address <= 0xEFFF)
	{
		LOG_L3("Work ram [0]: " +
		       std::string("Read ") +
		       ToHex(_workRam[0][address - 0xE000]) +
		       std::string(" from address ") +
		       ToHex(address));
		
		return _workRam[0][address - 0xE000];
	}
	else if (address >= 0xF000 && address <= 0xFDFF)
	{
		LOG_L3("Work ram [" + ToDec(_selectedWorkRamBank) + "]: " +
		       std::string("Read ") +
		       ToHex(_workRam[_selectedWorkRamBank][address - 0xF000]) +
		       std::string(" from address ") +
		       ToHex(address));
		
		return _workRam[_selectedWorkRamBank][address - 0xF000];
	}
	else if (address >= 0xFE00 && address <= 0xFE9F)
	{
		LOG_L3("OAM: " +
		       std::string("Read ") +
		       ToHex(_cartridge->ReadByte(address)) +
		       std::string(" from address ") +
		       ToHex(address));
		
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
				       (_lcdMode & 0x03) |
				       (_coincidenceFlag ? 0x04 : 0x00);
			
			case 0xFF44:
				// lcd y
				return _lcdY;
			
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
		LOG_L3("High ram: " +
		       std::string("Read ") +
		       ToHex(_highRam[address - 0xFF80]) +
		       std::string(" from address ") +
		       ToHex(address));
		
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
				
			for (int i = 0; i < _changedTiles.size(); i++)
			{
				if (changedTile[0] == _changedTiles[i][0] &&
				    changedTile[1] == _changedTiles[i][1])
				{
					isAlreadyInList = GBC_TRUE;
					break;
				}
			}
			
			if (!isAlreadyInList)
			{
				_changedTiles.push_back(changedTile);
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
				
				for (int i = 0; i < _changedBackgroundMapElements.size(); i++)
				{
					if (changedBackgroundMapElement[0] == _changedBackgroundMapElements[i][0] &&
					    changedBackgroundMapElement[1] == _changedBackgroundMapElements[i][1])
					{
						isAlreadyInList = GBC_TRUE;
						break;
					}
				}
				
				if (!isAlreadyInList)
				{
					_changedBackgroundMapElements.push_back(changedBackgroundMapElement);
				}
			}
			else
			{
				int *changedBackgroundMapAttribute = new int[2];
				
				changedBackgroundMapAttribute[0] = (address <= 0x9BFF) ? 0 : 1;
				changedBackgroundMapAttribute[1] = (address <= 0x9BFF) ? (address - 0x9800) : (address - 0x9C00);
				
				int isAlreadyInList = GBC_FALSE;
				
				for (int i = 0; i < _changedBackgroundMapAttributes.size(); i++)
				{
					if (changedBackgroundMapAttribute[0] == _changedBackgroundMapAttributes[i][0] &&
					    changedBackgroundMapAttribute[1] == _changedBackgroundMapAttributes[i][1])
					{
						isAlreadyInList = GBC_TRUE;
						break;
					}
				}
				
				if (!isAlreadyInList)
				{
					_changedBackgroundMapAttributes.push_back(changedBackgroundMapAttribute);
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
		
		for (int i = 0; i < _changedSpriteAttributes.size(); i++)
		{
			if (changedSpriteAttribute == _changedSpriteAttributes[i])
			{
				isAlreadyInList = GBC_TRUE;
				break;
			}
		}
		
		if (!isAlreadyInList)
		{
			_changedSpriteAttributes.push_back(changedSpriteAttribute);
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
				
				_directionKeysSelected = !GET_BIT(value, 4);
				_buttonKeysSelected = !GET_BIT(value, 5);
				
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
				
				_timerStopped = GET_BIT(value, 2);
				
				break;
			
			case 0xFF0F:
				// interrupt flags
				_ioPorts[address - 0xFF00] = value;
				
				if (GET_BIT(value, 0))
				{
					_hybr1s80.SignalVBlankInterrupt();
				}
				
				if (GET_BIT(value, 1))
				{
					_hybr1s80.SignalLCDStatusInterrupt();
				}
				
				if (GET_BIT(value, 2))
				{
					_hybr1s80.SignalTimerInterrupt();
				}
				
				if (GET_BIT(value, 3))
				{
					_hybr1s80.SignalSerialInterrupt();
				}
				
				if (GET_BIT(value, 4))
				{
					_hybr1s80.SignalJoypadInterrupt();
				}
				
				break;
			
			case 0xFF40:
				// lcd control
				_ioPorts[address - 0xFF00] = value;
				
				_backgroundDisplayEnabled = GET_BIT(value, 0);
				_spriteDisplayEnabled = GET_BIT(value, 1);
				_spriteSize = GET_BIT(value, 2);
				_backgroundTileMapDisplaySelect = GET_BIT(value, 3);
				_backgroundAndWindowTileDataSelect = GET_BIT(value, 4);
				_windowDisplayEnabled = GET_BIT(value, 5);
				_windowTileMapDisplaySelect = GET_BIT(value, 6);
				_lcdDisplayEnabled = GET_BIT(value, 7);
				
				break;
			
			case 0xFF41:
				// lcd status register
				// doesn't have any effect
				_ioPorts[address - 0xFF00] = value;
				
				//_lcdMode = LCDMode(value & 0x03);
				//_coincidenceFlag = GET_BIT(value, 2);
				_hBlankInterruptEnabled = GET_BIT(value, 3);
				_vBlankInterruptEnabledInLCD = GET_BIT(value, 4);
				_oamInterruptEnabled = GET_BIT(value, 5);
				_coincidenceInterruptEnabled = GET_BIT(value, 6);
				
				break;
			
			case 0xFF42:
				// scroll y
				_ioPorts[address - 0xFF00] = value;
				_scrollY = value;
				
				break;
			
			case 0xFF43:
				// scroll x
				_ioPorts[address - 0xFF00] = value;
				_scrollX = value;
				
				break;
			
			case 0xFF44:
				// lcd y coordinate
				// doesn't have any effect
				_ioPorts[address - 0xFF00] = value;
				
				// reset counter
				_lcdY = 0;
				
				break;
			
			case 0xFF45:
				// lcd y compare
				_ioPorts[address - 0xFF00] = value;
				_lcdYCompare = value;
				
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
				
				_monochromeBackgroundPalette.colors[0] = _monochromePalette.colors[value & 0b00000011];
				_monochromeBackgroundPalette.colors[1] = _monochromePalette.colors[(value & 0b00001100) >> 2];
				_monochromeBackgroundPalette.colors[2] = _monochromePalette.colors[(value & 0b00110000) >> 4];
				_monochromeBackgroundPalette.colors[3] = _monochromePalette.colors[(value & 0b11000000) >> 6];
				
				break;
			
			case 0xFF48:
				// sprite palette 0 data
				_ioPorts[address - 0xFF00] = value;
				
				_monochromeSpritePalette0.colors[0] = _monochromePalette.colors[value & 0b00000011];
				_monochromeSpritePalette0.colors[1] = _monochromePalette.colors[(value & 0b00001100) >> 2];
				_monochromeSpritePalette0.colors[2] = _monochromePalette.colors[(value & 0b00110000) >> 4];
				_monochromeSpritePalette0.colors[3] = _monochromePalette.colors[(value & 0b11000000) >> 6];
				
				break;
			
			case 0xFF49:
				// sprite palette 1 data
				_ioPorts[address - 0xFF00] = value;
				
				_monochromeSpritePalette1.colors[0] = _monochromePalette.colors[value & 0b00000011];
				_monochromeSpritePalette1.colors[1] = _monochromePalette.colors[(value & 0b00001100) >> 2];
				_monochromeSpritePalette1.colors[2] = _monochromePalette.colors[(value & 0b00110000) >> 4];
				_monochromeSpritePalette1.colors[3] = _monochromePalette.colors[(value & 0b11000000) >> 6];
				
				break;
			
			case 0xFF4A:
				// window y
				_ioPorts[address - 0xFF00] = value;
				_windowY = value;
				
				break;
			
			case 0xFF4B:
				// lcd y compare
				_ioPorts[address - 0xFF00] = value;
				_windowX = value - 7;
				
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
				
				if (GET_BIT(value, 7) && !_hBlankDMATransferActive)
				{
					int source = JOIN_BYTES(_ioPorts[0xFF51 - 0xFF00], _ioPorts[0xFF52 - 0xFF00]) & 0x000F;
					int destination = JOIN_BYTES(_ioPorts[0xFF53 - 0xFF00], _ioPorts[0xFF54 - 0xFF00]) & 0xE00F;
					int length = ((value & 0x7F) + 1) * 0x10;
				
					_hBlankDMATransferActive = GBC_TRUE;
					_hBlankDMATransferSourceAddress = source;
					_hBlankDMATransferDestinationAddress = destination;
					_hBlankDMATransferLength = length;
					_currentHBlankDMATransferAddressOffset = 0x0000;
					
					_ioPorts[address - 0xFF00] = SET_BIT(_ioPorts[address - 0xFF00], 7, GBC_FALSE);
				}
				else if (!GET_BIT(value, 7) && _hBlankDMATransferActive)
				{
					_hBlankDMATransferActive = GBC_FALSE;
					_ioPorts[address - 0xFF00] = SET_BIT(_ioPorts[address - 0xFF00], 7, GBC_TRUE);
				}
				else
				{
					int source = JOIN_BYTES(_ioPorts[0xFF51 - 0xFF00], _ioPorts[0xFF52 - 0xFF00]) & 0x000F;
					int destination = JOIN_BYTES(_ioPorts[0xFF53 - 0xFF00], _ioPorts[0xFF54 - 0xFF00]) & 0xE00F;
					int length = ((value & 0x7F) + 1) * 0x10;
					
					for (int i = 0; i < length; i++)
					{
						WriteByte(destination + i, ReadByte(source + i));
					}
					
					// emulate bit 7 for reading, so the dma transfer "takes some time"
					// _ioPorts[0xFF55 - 0xFF00] = SET_BIT(_ioPorts[0xFF55 - 0xFF00], 7, GBC_TRUE);
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
				
				if (GET_BIT(value, 7)) _colorBackgroundPaletteIndexAutoIncrement = GBC_TRUE;
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
					
					_colorBackgroundPalettes[palette].colors[color].red = value & 0b00011111;
					_colorBackgroundPalettes[palette].colors[color].green &= 0b00011000;
					_colorBackgroundPalettes[palette].colors[color].green |= (value >> 5) & 0b00000111;
				}
				else
				{
					int palette = (_ioPorts[0xFF68 - 0xFF00] & 0x3F) / 8;
					int color = ((_ioPorts[0xFF68 - 0xFF00] & 0x3F) % 8) / 2;
					
					_colorBackgroundPalettes[palette].colors[color].green &= 0b00000111;
					_colorBackgroundPalettes[palette].colors[color].green |= (value << 3) & 0b00011000;
					_colorBackgroundPalettes[palette].colors[color].blue = (value >> 2) & 0b00011111;
				}
				
				if (_colorBackgroundPaletteIndexAutoIncrement) _ioPorts[0xFF68 - 0xFF00]++; // take care because of bti 7
				
				break;
			
			case 0xFF6A:
				// sprite palette index
				_ioPorts[address - 0xFF00] = value;
				
				if (GET_BIT(value, 7)) _colorSpritePaletteIndexAutoIncrement = GBC_TRUE;
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
					
					_colorSpritePalettes[palette].colors[color].red = value & 0b00011111;
					_colorSpritePalettes[palette].colors[color].green &= 0b00011000;
					_colorSpritePalettes[palette].colors[color].green |= (value >> 5) & 0b00000111;
				}
				else
				{
					int palette = (_ioPorts[0xFF6A - 0xFF00] & 0x3F) / 8;
					int color = ((_ioPorts[0xFF6A - 0xFF00] & 0x3F) % 8) / 2;
					
					_colorSpritePalettes[palette].colors[color].green &= 0b00000111;
					_colorSpritePalettes[palette].colors[color].green |= (value << 3) & 0b00011000;
					_colorSpritePalettes[palette].colors[color].blue = (value >> 2) & 0b00011111;
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
	else if (address == 0xFFFF)
	{
		// interrupt enable
		_interruptEnableRegister = value;
		
		_vBlankInterruptEnabled = GET_BIT(value, 0);
		_lcdStatusInterruptEnabled = GET_BIT(value, 1);
		_timerInterruptEnabled = GET_BIT(value, 2);
		_serialInterruptEnabled = GET_BIT(value, 3);
		_joypadInterruptEnabled = GET_BIT(value, 4);
	}
	else
	{
		LOG(std::string("MemoryMap: Address out of range: ") + ToHex(address));
	}
}

void gbc::core::GameboyColor::DoOAMSearch()
{
	_lcdMode = searchingOAM;
	
	_coincidenceFlag = (_lcdY == _lcdYCompare);
	
	if (_oamInterruptEnabled)
	{
		_ioPorts[0xFF0F - 0xFF00] = SET_BIT(_ioPorts[0xFF0F - 0xFF00], 1, GBC_TRUE);
		_hybr1s80.SignalLCDStatusInterrupt();
	}
	
	if (_coincidenceInterruptEnabled && _coincidenceFlag)
	{
		_ioPorts[0xFF0F - 0xFF00] = SET_BIT(_ioPorts[0xFF0F - 0xFF00], 1, GBC_TRUE);
		_hybr1s80.SignalLCDStatusInterrupt();
	}
	
	UpdateSpriteAttributes();
}

void gbc::core::GameboyColor::DoTransferData()
{
	_lcdMode = transferringData;
	
	UpdateTiles();
	UpdateBackgroundMapElements();
	UpdateBackgroundMapAttributes();
	UpdateSpriteAttributes();
	
	if (_lcdDisplayEnabled)
	{
		if ((_cartridge->GetHeader().platformSupport == cartridges::gameboyColorSupport ||
		    _cartridge->GetHeader().platformSupport == cartridges::gameboyColorOnly) &&
		    !_forceClassicGameboy)
		{
			if (!_backgroundDisplayEnabled)
			{
				DrawBackgroundMap(COLOR_0 | COLOR_1 | COLOR_2 | COLOR_3);
				DrawWindowMap(COLOR_0 | COLOR_1 | COLOR_2 | COLOR_3);
				DrawSprites(COLOR_0 | COLOR_1 | COLOR_2 | COLOR_3);
			}
			else
			{
				DrawBackgroundMap(COLOR_0, UseOAMPriorityBit);
				
				if (_windowDisplayEnabled)
				{
					DrawWindowMap(COLOR_0, UseOAMPriorityBit);
				}
				
				DrawSprites(COLOR_1 | COLOR_2 | COLOR_3, spriteBehindBackground);
				DrawBackgroundMap(COLOR_1 | COLOR_2 | COLOR_3, UseOAMPriorityBit);
				
				if (_windowDisplayEnabled)
				{
					DrawWindowMap(COLOR_1 | COLOR_2 | COLOR_3, UseOAMPriorityBit);
				}
				
				DrawSprites(COLOR_1 | COLOR_2 | COLOR_3, spriteAboveBackground);
				DrawBackgroundMap(COLOR_1 | COLOR_2 | COLOR_3, BackgroundPriority);
				
				if (_windowDisplayEnabled)
				{
					DrawWindowMap(COLOR_1 | COLOR_2 | COLOR_3, BackgroundPriority);
				}
			}
		}
		else
		{
			if (_backgroundDisplayEnabled)
			{
				DrawBackgroundMap(COLOR_0);
			}
			
			if (_windowDisplayEnabled)
			{
				//DrawWindowMap(COLOR_0);
			}
			
			DrawSprites(COLOR_1 | COLOR_2 | COLOR_3, spriteBehindBackground);
			
			if (_backgroundDisplayEnabled)
			{
				DrawBackgroundMap(COLOR_1 | COLOR_2 | COLOR_3);
			}
			
			if (_windowDisplayEnabled)
			{
				//DrawWindowMap(COLOR_1 | COLOR_2 | COLOR_3);
			}
			
			DrawSprites(COLOR_1 | COLOR_2 | COLOR_3, spriteAboveBackground);
		}
	}
	
	if (_lcd)
	{
		_lcd->DrawScanline(Scanline(_lcdY, _currentScanline));
	}
	else
	{
		ERROR("GameboyColor: No LCD set.");
	}
}

void gbc::core::GameboyColor::DoHBlank()
{
	_lcdMode = hBlank;
	
	if (_hBlankInterruptEnabled)
	{
		_ioPorts[0xFF0F - 0xFF00] = SET_BIT(_ioPorts[0xFF0F - 0xFF00], 1, GBC_TRUE);
		_hybr1s80.SignalLCDStatusInterrupt();
	}
	
	if (_hBlankDMATransferActive)
	{
		if (_currentHBlankDMATransferAddressOffset < _hBlankDMATransferLength)
		{
			int currentSource = _hBlankDMATransferSourceAddress + _currentHBlankDMATransferAddressOffset;
			int currentDestination = _hBlankDMATransferDestinationAddress + _currentHBlankDMATransferAddressOffset;
			
			for (int i = 0; i < 0x10; i++)
			{
				WriteByte(currentDestination + i, ReadByte(currentSource + i));
			}
			
			_currentHBlankDMATransferAddressOffset += 0x10;
			
			_ioPorts[0xFF55 - 0xFF00] &= 0x80;
			_ioPorts[0xFF55 - 0xFF00] |= (((_hBlankDMATransferLength - _currentHBlankDMATransferAddressOffset) / 0x10) - 1) & 0x7F;
		}
		else
		{
			_ioPorts[0xFF55 - 0xFF00] = 0xFF;

			_hBlankDMATransferActive = GBC_FALSE;
			_hBlankDMATransferSourceAddress = 0x0000;
			_hBlankDMATransferDestinationAddress = 0x0000;
			_hBlankDMATransferLength = 0x0000;
			_currentHBlankDMATransferAddressOffset = 0x0000;
		}
	}

	if (_lcdY < 144)
	{
		_lcdY++;
	}
}

void gbc::core::GameboyColor::DoVBlank()
{
	_lcdMode = vBlank;
	
	_coincidenceFlag = (_lcdY == _lcdYCompare);
	
	if (_coincidenceInterruptEnabled && _coincidenceFlag)
	{
		_ioPorts[0xFF0F - 0xFF00] = SET_BIT(_ioPorts[0xFF0F - 0xFF00], 1, GBC_TRUE);
	}
	
	if (_vBlankInterruptEnabled && !_vBlankInterruptAlreadyRequested)
	{
		_ioPorts[0xFF0F - 0xFF00] = SET_BIT(_ioPorts[0xFF0F - 0xFF00], 0, GBC_TRUE);
		_hybr1s80.SignalVBlankInterrupt();
		_vBlankInterruptAlreadyRequested = GBC_TRUE;
	}
	
	if (_lcdY >= 144)
	{
		_lcdY++;
	}
	
	if (_lcdY > 153)
	{
		if (_lcd)
		{
			_lcd->FinishFrame();
		}
		
		_lcdY = 0;
		_vBlankInterruptAlreadyRequested = GBC_FALSE;
	}
}

void gbc::core::GameboyColor::UpdateTiles()
{
	int lastTile = 0;
	
	while (_changedTiles.size() > 0)
	{
		int videoRamBank = _changedTiles.back()[0];
		int tileNumber = _changedTiles.back()[1];
		int videoRamAddress = tileNumber << 4; // * 0x10
		
		for (int y = 0; y < 8; y++)
		{
			int colorNumbersLow = _videoRam[videoRamBank][videoRamAddress + y * 2];
			int colorNumbersHigh = _videoRam[videoRamBank][videoRamAddress + y * 2 + 1];
			
			for (int x = 0; x < 8; x++)
			{
				_tiles[videoRamBank][tileNumber].data[x][y] = (((colorNumbersHigh << 1) >> (7 - x)) & 0b10) |
				                                              ((colorNumbersLow >> (7 - x)) & 0b01);
			}
		}
		
		delete[] _changedTiles.back();
		
		_changedTiles.pop_back();
	}
}

void gbc::core::GameboyColor::UpdateBackgroundMapElements()
{
	while (_changedBackgroundMapElements.size() > 0)
	{
		int mapNumber = _changedBackgroundMapElements.back()[0];
		int mapElementNumber = _changedBackgroundMapElements.back()[1];
		
		_backgroundMapElements[mapNumber][mapElementNumber] = _videoRam[0][(mapNumber == 0) ?
		                                                      (0x9800 - 0x8000 + mapElementNumber) :
		                                                      (0x9C00 - 0x8000 + mapElementNumber)];
		
		delete[] _changedBackgroundMapElements.back();
		
		_changedBackgroundMapElements.pop_back();
	}
}

void gbc::core::GameboyColor::UpdateBackgroundMapAttributes()
{
	while (_changedBackgroundMapAttributes.size() > 0)
	{
		int mapNumber = _changedBackgroundMapAttributes.back()[0];
		int mapElementNumber = _changedBackgroundMapAttributes.back()[1];
		
		int backgroundMapAttribute = _videoRam[1][(mapNumber == 0) ?
		                             (0x9800 + mapElementNumber) :
		                             (0x9C00 + mapElementNumber)];
		
		_backgroundMapAttributes[mapNumber][mapElementNumber].backgroundColorPaletteNumber = backgroundMapAttribute & 0x07;
		_backgroundMapAttributes[mapNumber][mapElementNumber].tileVideoRamBankNumber = (backgroundMapAttribute >> 3) & 0x01;
		_backgroundMapAttributes[mapNumber][mapElementNumber].horizontalFlip = HorizontalFlip((backgroundMapAttribute >> 5) & 0x01);
		_backgroundMapAttributes[mapNumber][mapElementNumber].verticalFlip = VerticalFlip((backgroundMapAttribute >> 6) & 0x01);
		_backgroundMapAttributes[mapNumber][mapElementNumber].backgroundToOAMPriority = BackgroundToOAMPriority((backgroundMapAttribute >> 7) & 0x01);
		
		delete[] _changedBackgroundMapAttributes.back();
		
		_changedBackgroundMapAttributes.pop_back();
	}
}

void gbc::core::GameboyColor::UpdateSpriteAttributes()
{
	while (_changedSpriteAttributes.size() > 0)
	{
		int spriteAttributeNumber = _changedSpriteAttributes.back();
		int oamAddress = spriteAttributeNumber << 2; // * 4
		
		int spriteAttributeFlags = _oam[oamAddress + 3];
		
		_spriteAttributes[spriteAttributeNumber].y = _oam[oamAddress] - 16;
		_spriteAttributes[spriteAttributeNumber].x = _oam[oamAddress + 1] - 8;
		_spriteAttributes[spriteAttributeNumber].tileNumber = _oam[oamAddress + 2];
		
		_spriteAttributes[spriteAttributeNumber].spriteColorPaletteNumber = spriteAttributeFlags & 0x07;
		_spriteAttributes[spriteAttributeNumber].tileVideoRamBankNumber = (spriteAttributeFlags >> 3) & 0x01;
		_spriteAttributes[spriteAttributeNumber].spriteMonochromePaletteNumber = (spriteAttributeFlags >> 4) & 0x01;
		_spriteAttributes[spriteAttributeNumber].horizontalFlip = HorizontalFlip((spriteAttributeFlags >> 5) & 0x01);
		_spriteAttributes[spriteAttributeNumber].verticalFlip = VerticalFlip((spriteAttributeFlags >> 6) & 0x01);
		_spriteAttributes[spriteAttributeNumber].spriteToBackgroundPriority = SpriteToBackgroundPriority((spriteAttributeFlags >> 7) & 0x01);
		
		/*std::cout << "updated sprite attribute, tilenumber nao: " << ToDec(_spriteAttributes[spriteAttributeNumber].tileNumber) << std::endl << std::endl;*/
		
		_changedSpriteAttributes.pop_back();
	}
}

void gbc::core::GameboyColor::DrawSprites(int enabledColors,
                                          SpriteToBackgroundPriority spriteToBackgroundPriority)
{
	for (int i = 0; i < 40; i++)
	{
		SpriteAttribute spriteAttribute = _spriteAttributes[i];
		
		if (spriteAttribute.spriteToBackgroundPriority == spriteToBackgroundPriority)
		{
			ColorPalette colorPalette = spriteAttribute.spriteMonochromePaletteNumber == 0 ?
			                            _monochromeSpritePalette0 :
			                            _monochromeSpritePalette1;
			
			if ((_cartridge->GetHeader().platformSupport == cartridges::gameboyColorSupport ||
			    _cartridge->GetHeader().platformSupport == cartridges::gameboyColorOnly) &&
			    !_forceClassicGameboy)
			{
				colorPalette = _colorSpritePalettes[spriteAttribute.spriteColorPaletteNumber];
			}
			
			if ((_cartridge->GetHeader().platformSupport == cartridges::gameboyColorSupport ||
			    _cartridge->GetHeader().platformSupport == cartridges::gameboyColorOnly) &&
			    !_forceClassicGameboy)
			{
				DrawTile(spriteAttribute.x, spriteAttribute.y,
						 _tiles[spriteAttribute.tileVideoRamBankNumber][(0x8000 - 0x8000) + spriteAttribute.tileNumber],
						 spriteAttribute.horizontalFlip,
						 spriteAttribute.verticalFlip,
						 colorPalette,
						 enabledColors);
			}
			else
			{
				DrawTile(spriteAttribute.x, spriteAttribute.y,
						 _tiles[0][(0x8000 - 0x8000) + spriteAttribute.tileNumber],
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
		SpriteAttribute spriteAttribute = _spriteAttributes[i];
		
		int tileNumber = (0x8000 - 0x8000) + spriteAttribute.tileNumber;
		
		ColorPalette colorPalette = spriteAttribute.spriteMonochromePaletteNumber == 0 ?
			                        _monochromeSpritePalette0 :
			                        _monochromeSpritePalette1;
		
		if ((_cartridge->GetHeader().platformSupport == cartridges::gameboyColorSupport ||
		    _cartridge->GetHeader().platformSupport == cartridges::gameboyColorOnly) &&
		    !_forceClassicGameboy)
		{
			colorPalette = _colorSpritePalettes[spriteAttribute.spriteColorPaletteNumber];
		}			
	    
		if ((_cartridge->GetHeader().platformSupport == cartridges::gameboyColorSupport ||
		    _cartridge->GetHeader().platformSupport == cartridges::gameboyColorOnly) &&
		    !_forceClassicGameboy)
		{
			DrawTile(spriteAttribute.x, spriteAttribute.y,
					 _tiles[spriteAttribute.tileVideoRamBankNumber][(0x8000 - 0x8000) + spriteAttribute.tileNumber],
					 spriteAttribute.horizontalFlip,
					 spriteAttribute.verticalFlip,
					 colorPalette,
					 enabledColors);
		}
		else
		{
			DrawTile(spriteAttribute.x, spriteAttribute.y,
					 _tiles[0][(0x8000 - 0x8000) + spriteAttribute.tileNumber],
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
	for (int mapX = 0; mapX < 32; mapX++)
	{
		int mapElementX = (mapX << 2 + _scrollX) >> 3;
		int mapElementY = (_lcdY + _scrollY) >> 3;
		
		if (mapElementX < 0)
		{
			mapElementX += 32;
		}
		
		if (mapElementY < 0)
		{
			mapElementY += 32;
		}
	
		mapElementX &= 0x1F; // % 32
		mapElementY &= 0x1F; // % 32
		
		int mapElementNumber = (mapElementY << 5) + mapElementX;
		
		BackgroundMapAttribute backgroundMapAttribute = _backgroundMapAttributes
			                                            [_backgroundTileMapDisplaySelect]
			                                            [mapElementNumber];
		
		if (backgroundMapAttribute.backgroundToOAMPriority == backgroundToOAMPriority)
		{
			DrawMapTile(mapX, -_scrollX, -_scrollY, 
			            _backgroundTileMapDisplaySelect,
			            enabledColors,
			            _cartridge->GetHeader().platformSupport);
		}
	}
}

void gbc::core::GameboyColor::DrawBackgroundMap(int enabledColors)
{
	for (int mapX = 0; mapX < 32; mapX++)
	{
		DrawMapTile(mapX, -_scrollX, -_scrollY, 
		            _backgroundTileMapDisplaySelect,
		            enabledColors,
		            _cartridge->GetHeader().platformSupport);
	}
}

void gbc::core::GameboyColor::DrawWindowMap(int enabledColors,
                                            BackgroundToOAMPriority backgroundToOAMPriority)
{
	for (int mapX = 0; mapX < 21; mapX++)
	{
		int mapElementX = (mapX << 3 - _windowX) >> 3;
		int mapElementY = (_lcdY - _windowY) >> 3;
		
		if (mapElementX < 0)
		{
			mapElementX += 32;
		}
		
		if (mapElementY < 0)
		{
			mapElementY += 32;
		}
	
		mapElementX &= 0x1F; // % 32
		mapElementY &= 0x1F; // % 32
		
		int mapElementNumber = (mapElementY << 5) + mapElementX;
		
		BackgroundMapAttribute backgroundMapAttribute = _backgroundMapAttributes
			                                            [_windowTileMapDisplaySelect]
			                                            [mapElementNumber];
		
		if (backgroundMapAttribute.backgroundToOAMPriority == backgroundToOAMPriority)
		{
			DrawMapTile(mapX, _windowX, _windowY,
			            _windowTileMapDisplaySelect,
			            enabledColors,
			            _cartridge->GetHeader().platformSupport);
		}
	}
}

void gbc::core::GameboyColor::DrawWindowMap(int enabledColors)
{
	for (int mapX = 0; mapX < 32; mapX++)
	{
		DrawMapTile(mapX, _windowX, _windowY,
		            _windowTileMapDisplaySelect,
		            enabledColors,
		            _cartridge->GetHeader().platformSupport);
	}
}

void gbc::core::GameboyColor::DrawMapTile(int mapX,
                                          int xOffset,
                                          int yOffset,
                                          int tileMapDisplaySelect,
                                          int enabledColors,
                                          cartridges::PlatformSupport platformSupport)
{
	//if (tileMapDisplaySelect == 0) return; ////////////////////
	int x = (mapX << 3) + xOffset; // x tile beginning
	int y = (_lcdY - yOffset) - ((_lcdY - yOffset) & 0x07) + yOffset; // y tile beginning // % 8
	
	int mapElementX = ((mapX << 3) - xOffset) >> 3; // / 8
	int mapElementY = (_lcdY - yOffset) >> 3;       // / 8
	
	if (mapElementX < 0)
	{
		mapElementX += 32;
	}
	
	if (mapElementY < 0)
	{
		mapElementY += 32;
	}
	
	mapElementX &= 0x1F; // % 32
	mapElementY &= 0x1F; // % 32
	
	int mapElementNumber = (mapElementY << 5) + mapElementX;
	
	BackgroundMapAttribute backgroundMapAttribute = _backgroundMapAttributes
		                                            [tileMapDisplaySelect]
		                                            [mapElementNumber];
	
	int backgroundMapElement = _backgroundMapElements
		                       [tileMapDisplaySelect]
		                       [mapElementNumber];
	
	int tileVideoRamBankNumber = 0;
	
	if ((_cartridge->GetHeader().platformSupport == cartridges::gameboyColorSupport ||
	    _cartridge->GetHeader().platformSupport == cartridges::gameboyColorOnly) &&
	    !_forceClassicGameboy)
	{
		tileVideoRamBankNumber = backgroundMapAttribute.tileVideoRamBankNumber;
	}
	
	int tileNumber = (!_backgroundAndWindowTileDataSelect) ?
		             (((0x9000 - 0x8000) / 16) + GET_SIGNED_VALUE(backgroundMapElement)) :
		             (((0x8000 - 0x8000) / 16) + backgroundMapElement);
	
	Tile tile = _tiles[tileVideoRamBankNumber][tileNumber];
	
	ColorPalette colorPalette = _monochromeBackgroundPalette;
	
	if ((_cartridge->GetHeader().platformSupport == cartridges::gameboyColorSupport ||
	    _cartridge->GetHeader().platformSupport == cartridges::gameboyColorOnly) &&
	    !_forceClassicGameboy)
	{
		colorPalette = _colorBackgroundPalettes[backgroundMapAttribute.backgroundColorPaletteNumber];
	}
	
	if ((_cartridge->GetHeader().platformSupport == cartridges::gameboyColorSupport ||
	    _cartridge->GetHeader().platformSupport == cartridges::gameboyColorOnly) &&
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
			     notHorizontallyFlipped,
			     notVerticallyFlipped,
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
	int tileY = _lcdY - y;
	
	if (tileY >= 0 && tileY < 8)
	{
		for (int tileX = 0; tileX < 8; tileX++)
		{
			int scanlinePosition = x + tileX;
			
			if (scanlinePosition >= 0 && scanlinePosition < 160)
			{
				int realTileX = tileX;
				int realTileY = tileY;
				
				if (horizontalFlip == horizontallyFlipped)
				{
					realTileX = 7 - tileX;
				}
				
				if (verticalFlip == verticallyFlipped)
				{
					realTileY = 7 - tileY;
				}
				
				int colorNumber = tile.data[realTileX][realTileY];
				
				if (((colorNumber == 0) && (enabledColors & COLOR_0)) ||
				    ((colorNumber == 1) && (enabledColors & COLOR_1)) ||
				    ((colorNumber == 2) && (enabledColors & COLOR_2)) ||
				    ((colorNumber == 3) && (enabledColors & COLOR_3)))
				{
					_currentScanline[scanlinePosition] = colorPalette.colors[colorNumber];
				}
			}
		}
	}
}
