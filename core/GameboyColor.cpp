#include "GameboyColor.hpp"

gbc::core::GameboyColor::GameboyColor()
	: _lcd(NULL), _wtf80()
{
}

gbc::core::GameboyColor::~GameboyColor()
{
}

void gbc::core::GameboyColor::Initialize()
{
	_wtf80.SetMemoryBus(this);
	_wtf80.Step();
}

void gbc::core::GameboyColor::SetLCD(ILCD *lcd)
{
	_lcd = lcd;
}

void gbc::core::GameboyColor::RenderScanline()
{
	if (_lcdY < 144)
	{
		DoOAMSearch();
		ExecuteMachineClocks(80 * 10 * _speedFactor);
		
		DoTransferData();
		ExecuteMachineClocks(172 * 10 * _speedFactor);
		
		DoHBlank();
		ExecuteMachineClocks(204 * 10 * _speedFactor);
	}
	else
	{
		DoVBlank();
		ExecuteMachineClocks(456 * 10 * _speedFactor);
	}
}

void gbc::core::GameboyColor::RenderFrame()
{
	for (int i = 0; i < 153; i++)
	{	
		RenderScanline();
	}
}

void gbc::core::GameboyColor::ExecuteMachineClocks(int clocks)
{
	for (int i = 0; i < clocks * 4; i++)
	{
		_wtf80.Step();
		
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
					_timerInterruptRequested = GBC_TRUE;
				}
				else
				{
					_ioPorts[0xFF05 - 0xFF00]++;
				}
				
				_timerCounter = 0;
			}
			else _timerCounter++;
		}
	}
}

int gbc::core::GameboyColor::ReadByte(int address)
{
	if (address >= 0x0000 && address <= 0x7FFF)
	{
#ifdef DEBUG
		Log("Not implemented");
#endif
		return 0;
	}
	else if (address >= 0x8000 && address <= 0x9FFF)
	{
		return _videoRam[_selectedVideoRamBank][address - 0x8000];
	}
	else if (address >= 0xA000 && address <= 0xBFFF)
	{
#ifdef DEBUG
		Log("Not implemented");
#endif
		return 0;
	}
	else if (address >= 0xC000 && address <= 0xFDFF)
	{
		return _workRam[_selectedWorkRamBank][address - 0xC000];
	}
	else if (address >= 0xFE00 && address <= 0xFE9F)
	{
		return _oam[address - 0xFE00];
	}
	else if (address >= 0xFF00 && address <= 0xFF7F)
	{
		switch (address)
		{
			case 0xFF41:
				// lcd status
				return (_ioPorts[0xFF41 - 0xFF00] & 0xF8) |
				       (_lcdMode & 0x03) |
				       (_coincidenceFlag ? 0x04 : 0x00);
			
			case 0xFF44:
				// lcd y
				return _lcdY;
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

#ifdef DEBUG
	Log("Out of range.");
#endif
		
	return 0;
}

void gbc::core::GameboyColor::WriteByte(int address, int value)
{
	if (address >= 0x0000 && address <= 0x7FFF)
	{
#ifdef DEBUG
		Log("Not implemented");
#endif
	}
	else if (address >= 0x8000 && address <= 0x9FFF)
	{
		if (address <= 0x97FF)
		{
			int changedTile[2] = { _selectedVideoRamBank, (address - 0x8000) / 0x10 };
			
			if (std::find(_changedTiles.begin(),
			              _changedTiles.end(),
			              changedTile) == _changedTiles.end())
			{
				_changedTiles.push_back(changedTile);
			}
		}
		else
		{
			if (_selectedVideoRamBank == 1)
			{
				int changedBackgroundMapAttribute = address - 0x9800;
			
				if (std::find(_changedBackgroundMapAttributes.begin(),
				              _changedBackgroundMapAttributes.end(),
				              changedBackgroundMapAttribute) == _changedBackgroundMapAttributes.end())
				{
					_changedBackgroundMapAttributes.push_back(changedBackgroundMapAttribute);
				}
			}
		}
		
		_videoRam[_selectedVideoRamBank][address - 0x8000] = value;
	}
	else if (address >= 0xA000 && address <= 0xBFFF)
	{
#ifdef DEBUG
		Log("Not implemented");
#endif
	}
	else if (address >= 0xC000 && address <= 0xFDFF)
	{
		_workRam[_selectedWorkRamBank][address - 0xC000] = value;
	}
	else if (address >= 0xFE00 && address <= 0xFE9F)
	{
		int changedSpriteAttribute = (address - 0xFE00) / 4;
			
		if (std::find(_changedSpriteAttributes.begin(),
		              _changedSpriteAttributes.end(),
		              changedSpriteAttribute) == _changedSpriteAttributes.end())
		{
			_changedSpriteAttributes.push_back(changedSpriteAttribute);
		}
		
		_oam[address - 0xFE00] = value;
	}
	else if (address >= 0xFF00 && address <= 0xFF7F)
	{
		switch (address)
		{
			case 0xFF00:
				// joypad input
				_ioPorts[address - 0xFF00] = value;
				
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
				// interrupt flag
				_ioPorts[address - 0xFF00] = value;
				
				_vBlankInterruptRequested = GET_BIT(value, 0);
				_lcdStatusInterruptRequested = GET_BIT(value, 1);
				_timerInterruptRequested = GET_BIT(value, 2);
				_serialInterruptRequested = GET_BIT(value, 3);
				_joypadInterruptRequested = GET_BIT(value, 4);
				
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
				
				for (int i = 0; i < 0x80; i++)
				{
					WriteByte(0xFF00 + i, ReadByte((value << 8) + i));
				}
				
				break;
			
			case 0xFF47:
				// background palette data
				_ioPorts[address - 0xFF00] = value;
				
				_monochromeBackgroundPaletteData[0] = value & 0b00000011;
				_monochromeBackgroundPaletteData[1] = value & 0b00001100;
				_monochromeBackgroundPaletteData[2] = value & 0b00110000;
				_monochromeBackgroundPaletteData[3] = value & 0b11000000;
				
				break;
			
			case 0xFF48:
				// sprite palette 0 data
				_ioPorts[address - 0xFF00] = value;
				
				_monochromeSpritePalette0Data[0] = value & 0b00000011;
				_monochromeSpritePalette0Data[1] = value & 0b00001100;
				_monochromeSpritePalette0Data[2] = value & 0b00110000;
				_monochromeSpritePalette0Data[3] = value & 0b11000000;
				
				break;
			
			case 0xFF49:
				// sprite palette 1 data
				_ioPorts[address - 0xFF00] = value;
				
				_monochromeSpritePalette1Data[0] = value & 0b00000011;
				_monochromeSpritePalette1Data[1] = value & 0b00001100;
				_monochromeSpritePalette1Data[2] = value & 0b00110000;
				_monochromeSpritePalette1Data[3] = value & 0b11000000;
				
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
				_ioPorts[address - 0xFF00] = value;
				
				if ((((_ioPorts[0xFF68 - 0xFF00] & 0x3F) % 8) % 2) == 0)
				{
					int palette = (_ioPorts[0xFF68 - 0xFF00] & 0x3F) / 8;
					int color = ((_ioPorts[0xFF68 - 0xFF00] & 0x3F) % 8) / 2;
					
					_colorBackgroundPalettes[palette][color].red = value & 0b00011111;
					_colorBackgroundPalettes[palette][color].green &= 0b00011000;
					_colorBackgroundPalettes[palette][color].green |= (value >> 5) & 0b00000111;
				}
				else
				{
					int palette = (_ioPorts[0xFF68 - 0xFF00] & 0x3F) / 8;
					int color = ((_ioPorts[0xFF68 - 0xFF00] & 0x3F) % 8) / 2;
					
					_colorBackgroundPalettes[palette][color].green &= 0b00000111;
					_colorBackgroundPalettes[palette][color].green |= (value << 3) & 0b00011000;
					_colorBackgroundPalettes[palette][color].blue = (value >> 2) & 0b00011111;
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
				_ioPorts[address - 0xFF00] = value;
				
				if ((((_ioPorts[0xFF6A - 0xFF00] & 0x3F) % 8) % 2) == 0)
				{
					int palette = (_ioPorts[0xFF6A - 0xFF00] & 0x3F) / 8;
					int color = ((_ioPorts[0xFF6A - 0xFF00] & 0x3F) % 8) / 2;
					
					_colorSpritePalettes[palette][color].red = value & 0b00011111;
					_colorSpritePalettes[palette][color].green &= 0b00011000;
					_colorSpritePalettes[palette][color].green |= (value >> 5) & 0b00000111;
				}
				else
				{
					int palette = (_ioPorts[0xFF6A - 0xFF00] & 0x3F) / 8;
					int color = ((_ioPorts[0xFF6A - 0xFF00] & 0x3F) % 8) / 2;
					
					_colorSpritePalettes[palette][color].green &= 0b00000111;
					_colorSpritePalettes[palette][color].green |= (value << 3) & 0b00011000;
					_colorSpritePalettes[palette][color].blue = (value >> 2) & 0b00011111;
				}
				
				if (_colorSpritePaletteIndexAutoIncrement) _ioPorts[0xFF6A - 0xFF00]++; // take care because of bti 7
			
			case 0xFF70:
				// wram bank
				_ioPorts[address - 0xFF00] = value;
				
				if ((value & 0x07) == 0x00) _selectedWorkRamBank = 0x01;
				else _selectedWorkRamBank = (value & 0x07);
				
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
#ifdef DEBUG
		Log("Out of range.");
#endif
	}
}

void gbc::core::GameboyColor::DoOAMSearch()
{
	_lcdMode = searchingOAM;
	
	_coincidenceFlag = (_lcdY == _lcdYCompare);
	
	if (_oamInterruptEnabled)
	{
		_ioPorts[0xFF0F - 0xFF00] = SET_BIT(_ioPorts[0xFF0F - 0xFF00], 1, GBC_TRUE);
		_lcdStatusInterruptRequested = GBC_TRUE;
	}
	
	if (_coincidenceInterruptEnabled && _coincidenceFlag)
	{
		_ioPorts[0xFF0F - 0xFF00] = SET_BIT(_ioPorts[0xFF0F - 0xFF00], 1, GBC_TRUE);
		_lcdStatusInterruptRequested = GBC_TRUE;
	}
	
	UpdateSpriteAttributes();
}

void gbc::core::GameboyColor::DoTransferData()
{
	_lcdMode = transferringData;
}

void gbc::core::GameboyColor::DoHBlank()
{
	_lcdMode = hBlank;
	
	if (_hBlankInterruptEnabled)
	{
		_ioPorts[0xFF0F - 0xFF00] = SET_BIT(_ioPorts[0xFF0F - 0xFF00], 1, GBC_TRUE);
		_lcdStatusInterruptRequested = GBC_TRUE;	
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
		_vBlankInterruptRequested = GBC_TRUE;
		_vBlankInterruptAlreadyRequested = GBC_TRUE;
	}
	
	if (_lcdY >= 144)
	{
		_lcdY++;
	}
	
	if (_lcdY >= 153)
	{
		_lcdY = 0;
		_vBlankInterruptAlreadyRequested = GBC_FALSE;
	}
}

void gbc::core::GameboyColor::UpdateTiles()
{
	while (_changedTiles.size() > 0)
	{
		int videoRamBank = _changedTiles.back()[0];
		int tileNumber = _changedTiles.back()[1];
		int videoRamAddress = tileNumber * 0x10;
		
		for (int y = 0; y < 8; y++)
		{
			int colorNumbersLow = _videoRam[videoRamBank][videoRamAddress + y * 2];
			int colorNumbersHigh = _videoRam[videoRamBank][videoRamAddress + y * 2 + 1];
			
			for (int x = 0; x < 8; x++)
			{
				_tiles[videoRamBank][tileNumber][x][y] = (((colorNumbersHigh << 1) >> x) & 0b10) |
				                                         ((colorNumbersLow >> x) & 0b01);
			}
		}
		
		delete[] _changedTiles.back();
		
		_changedTiles.pop_back();
	}
}

void gbc::core::GameboyColor::UpdateBackgroundMapAttributes()
{
	while (_changedBackgroundMapAttributes.size() > 0)
	{
		int backgroundMapAttributeNumber = _changedBackgroundMapAttributes.back();
		int videoRamAddress = 0x9800 + backgroundMapAttributeNumber;
		
		int backgroundMapAttribute = _videoRam[1][videoRamAddress];
		
		_backgroundMapAttributes[backgroundMapAttributeNumber].backgroundColorPaletteNumber = backgroundMapAttribute & 0x07;
		_backgroundMapAttributes[backgroundMapAttributeNumber].tileVideoRamBankNumber = (backgroundMapAttribute >> 3) & 0x01;
		_backgroundMapAttributes[backgroundMapAttributeNumber].horizontalFlip = HorizontalFlip((backgroundMapAttribute >> 5) & 0x01);
		_backgroundMapAttributes[backgroundMapAttributeNumber].verticalFlip = VerticalFlip((backgroundMapAttribute >> 6) & 0x01);
		_backgroundMapAttributes[backgroundMapAttributeNumber].backgroundToOAMPriority = BackgroundToOAMPriority((backgroundMapAttribute >> 7) & 0x01);
		
		_changedBackgroundMapAttributes.pop_back();
	}
}

void gbc::core::GameboyColor::UpdateSpriteAttributes()
{
	while (_changedSpriteAttributes.size() > 0)
	{
		int spriteAttributeNumber = _changedSpriteAttributes.back();
		int oamAddress = spriteAttributeNumber * 4;
		
		int spriteAttributeFlags = _oam[oamAddress + 3];
		
		_spriteAttributes[spriteAttributeNumber].y = _oam[oamAddress];
		_spriteAttributes[spriteAttributeNumber].x = _oam[oamAddress + 1];
		_spriteAttributes[spriteAttributeNumber].tileNumber = _oam[oamAddress + 2];
		
		_spriteAttributes[spriteAttributeNumber].spriteColorPaletteNumber = spriteAttributeFlags & 0x07;
		_spriteAttributes[spriteAttributeNumber].tileVideoRamBankNumber = (spriteAttributeFlags >> 3) & 0x01;
		_spriteAttributes[spriteAttributeNumber].spriteMonochromePaletteNumber = (spriteAttributeFlags >> 4) & 0x01;
		_spriteAttributes[spriteAttributeNumber].horizontalFlip = HorizontalFlip((spriteAttributeFlags >> 5) & 0x01);
		_spriteAttributes[spriteAttributeNumber].verticalFlip = VerticalFlip((spriteAttributeFlags >> 6) & 0x01);
		_spriteAttributes[spriteAttributeNumber].spriteToBackgroundPriority = SpriteToBackgroundPriority((spriteAttributeFlags >> 7) & 0x01);
		
		_changedSpriteAttributes.pop_back();
	}
}
