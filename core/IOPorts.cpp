#include "IOPorts.hpp"

IOPorts::IOPorts()
	: MemoryBlock(IO_PORTS_BANKS, IO_PORTS_BANK_SIZE),
	  _oamDMATransferAddress(0),
	  _hdmaTransferActive(0),
	  _hdmaTransferSourceAddress(0),
	  _hdmaTransferDestinationAddress(0),
	  _hdmaTransferLength(0),
	  _hBlankHDMATransferCurrentOffset(0),
	  _selectedWorkRamBank(0),
	  _selectedVideoRamBank(0),
	  _directionKeysSelected(0),
	  _buttonKeysSelected(0),
	  _lcdDisplayEnabled(0),
	  _windowMapDisplaySelect(0),
	  _windowDisplayEnabled(0),
	  _backgroundAndWindowTileDataSelect(0),
	  _backgroundMapDisplaySelect(0),
	  _spriteSize(),
	  _spriteDisplayEnabled(0),
	  _backgroundDisplayEnabled(0),
	  _lcdMode(),
	  _coincidenceFlag(0),
	  _hBlankInterruptEnabled(0),
	  _vBlankInterruptEnabledInLCD(0),
	  _oamInterruptEnabled(0),
	  _coincidenceInterruptEnabled(0),
	  _scrollX(0),
	  _scrollY(0),
	  _lcdY(0),
	  _lcdYCompare(0),
	  _windowX(0),
	  _windowY(0),
	  _divider(0),
	  _timerCounter(0),
	  _timerModulo(0),
	  _timerClockFrequency(0),
	  _timerStopped(0),
	  _colorBackgroundPaletteIndexAutoIncrement(0),
	  _colorSpritePaletteIndexAutoIncrement(0)
{
	for (int i = 0; i < 4; i++)
	{
		_monochromePalette[i].SetRed(i * 0x08);
		_monochromePalette[i].SetGreen(i * 0x08);
		_monochromePalette[i].SetBlue(i * 0x08);
	}
	
	std::copy(_monochromePalette.begin(),
	          _monochromePalette.end(),
	          _monochromeBackgroundPalette.begin());
	
	for (int i = 0; i < 2; i++)
	{
		std::copy(_monochromePalette.begin(),
				  _monochromePalette.end(),
				  _monochromeSpritePalettes[i].begin());
	}
	
	for (int i = 0; i < 8; i++)
	{
		std::copy(_monochromePalette.begin(),
				  _monochromePalette.end(),
				  _colorBackgroundPalettes[i].begin());
	}
	
	for (int i = 0; i < 8; i++)
	{
		std::copy(_monochromePalette.begin(),
				  _monochromePalette.end(),
				  _colorSpritePalettes[i].begin());
	}
}

IOPorts::~IOPorts()
{
}

int IOPorts::ReadByte(int address)
{
	switch (address)
	{
		//case 0xFF00: return 0xFF;
		case 0xFF00:
			// joypad
			return ((!(_directionKeysSelected ? (*_joypad).GetRight() : (*_joypad).GetButtonA())) ? 0x01 : 0) |
			       ((!(_directionKeysSelected ? (*_joypad).GetLeft() : (*_joypad).GetButtonB())) ? 0x02 : 0) |
			       ((!(_directionKeysSelected ? (*_joypad).GetUp() : (*_joypad).GetSelect())) ? 0x04 : 0) |
			       ((!(_directionKeysSelected ? (*_joypad).GetDown() : (*_joypad).GetStart())) ? 0x08 : 0) |
			       ((!_directionKeysSelected) ? 0x10 : 0) |
			       ((!_buttonKeysSelected) ? 0x20 : 0);
			
		case 0xFF41:
			// lcd status
			return (ReadByteFromBank(0, 0xFF41 - IO_PORTS_OFFSET) & 0xF8) |
			       (GetEnumValue(_lcdMode) & 0x03) |
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
			
		default:
			return ReadByteFromBank(0, address - IO_PORTS_OFFSET);
	}
}

void IOPorts::WriteByte(int address, int value)
{
	switch (address)
	{
		case 0xFF00:
			// joypad input
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			_directionKeysSelected = !GetBit(value, 4);
			_buttonKeysSelected = !GetBit(value, 5);
			
			break;
			
		case 0xFF01:
			// serial transfer data
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			break;
			
		case 0xFF02:
			// serial transfer control
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			break;
			
		case 0xFF04:
			// write to devider register
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			break;
			
		case 0xFF05:
			// timer counter
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			break;
			
		case 0xFF06:
			// timer modulo
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			break;
			
		case 0xFF07:
			// timer control
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			switch (value & 0x03)
			{
				case 0x00:
					_timerClockFrequency = 1024;
					break;
					
				case 0x01:
					_timerClockFrequency = 16;
					break;
					
				case 0x02:
					_timerClockFrequency = 64;
					break;
					
				case 0x03:
					_timerClockFrequency = 256;
					break;
					
				default:
					break;
			}
			
			_timerStopped = !GetBit(value, 2);
			
			break;
			
		case InterruptHandler::INTERRUPT_REQUEST_ADDRESS:
			// interrupt request flags
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			break;
			
		case 0xFF40:
			// lcd control
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			_backgroundDisplayEnabled = GetBit(value, 0);
			_spriteDisplayEnabled = GetBit(value, 1);
			_spriteSize = SpriteSize(GetBit(value, 2));
			_backgroundMapDisplaySelect = GetBit(value, 3);
			_backgroundAndWindowTileDataSelect = GetBit(value, 4);
			_windowDisplayEnabled = GetBit(value, 5);
			_windowMapDisplaySelect = GetBit(value, 6);
			_lcdDisplayEnabled = GetBit(value, 7);
			
			break;
			
		case 0xFF41:
			// lcd status register
			// doesn't have any effect
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			//_rc.lcdMode = LCDMode(value & 0x03);
			//_rc.coincidenceFlag = GetBit(value, 2);
			_hBlankInterruptEnabled = GetBit(value, 3);
			_vBlankInterruptEnabledInLCD = GetBit(value, 4);
			_oamInterruptEnabled = GetBit(value, 5);
			_coincidenceInterruptEnabled = GetBit(value, 6);
			
			break;
			
		case 0xFF42:
			// scroll y
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			_scrollY = value;
			
			break;
			
		case 0xFF43:
			// scroll x
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			_scrollX = value;
			
			break;
			
		case 0xFF44:
			// lcd y coordinate
			// doesn't have any effect
			WriteByteToBank(0, address - IO_PORTS_OFFSET, 0x00);
			
			// reset counter
			_lcdY = 0x00;
			
			break;
			
		case 0xFF45:
			// lcd y compare
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			_lcdYCompare = value;
			
			break;
			
		case 0xFF46:
			// dma to oam transfer
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			_oamDMATransferAddress = value << 8;
			
			ExecuteOAMDMATransfer();
			
			break;
			
		case 0xFF47:
			// background palette data
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			_monochromeBackgroundPalette[0] = _monochromePalette[value & 0x03];
			_monochromeBackgroundPalette[1] = _monochromePalette[(value & 0x0C) >> 2];
			_monochromeBackgroundPalette[2] = _monochromePalette[(value & 0x30) >> 4];
			_monochromeBackgroundPalette[3] = _monochromePalette[(value & 0xC0) >> 6];
			
			break;
			
		case 0xFF48:
			// sprite palette 0 data
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			_monochromeSpritePalettes[0][0] = _monochromePalette[value & 3];
			_monochromeSpritePalettes[0][1] = _monochromePalette[(value & 12) >> 2];
			_monochromeSpritePalettes[0][2] = _monochromePalette[(value & 48) >> 4];
			_monochromeSpritePalettes[0][3] = _monochromePalette[(value & 192) >> 6];
			
			break;
			
		case 0xFF49:
			// sprite palette 1 data
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			_monochromeSpritePalettes[1][0] = _monochromePalette[value & 3];
			_monochromeSpritePalettes[1][1] = _monochromePalette[(value & 12) >> 2];
			_monochromeSpritePalettes[1][2] = _monochromePalette[(value & 48) >> 4];
			_monochromeSpritePalettes[1][3] = _monochromePalette[(value & 192) >> 6];
			
			break;
			
		case 0xFF4A:
			// window y
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			_windowY = value;
			
			break;
			
		case 0xFF4B:
			// lcd y compare
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			_windowX = value - 7;
			
			break;
			
		case 0xFF4D:
			// prepare speed switch
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			// to be implemented
			
			break;
			
		case 0xFF4F:
			// vram bank
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			// TODO: make platform check
			_selectedVideoRamBank = value & 0x01;
			
			break;
			
		case 0xFF51:
			// new dma source, high
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			break;
			
		case 0xFF52:
			// new dma source, low
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			break;
			
		case 0xFF53:
			// new dma destination, high
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			break;
			
		case 0xFF54:
			// new dma destination, low
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			break;
			
		case 0xFF55:
			// new dma length/mode/start
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			// TODO: something must be wrong here, check this later again...
			if (GetBit(value, 7) && !_hdmaTransferActive)
			{
				_hdmaMode = HDMAMode::HBLANK_HDMA;
				
				_hdmaTransferSourceAddress = JoinBytes(ReadByteFromBank(0, 0xFF51 - IO_PORTS_OFFSET),
				                                       ReadByteFromBank(0, 0xFF52 - IO_PORTS_OFFSET)) & 0xFFF0;
				
				_hdmaTransferDestinationAddress = (JoinBytes(ReadByteFromBank(0, 0xFF53 - IO_PORTS_OFFSET),
				                                             ReadByteFromBank(0, 0xFF54 - IO_PORTS_OFFSET)) & 0x1FF0) | 0x8000;
				
				_hdmaTransferLength = ((value & 0x7F) + 1) * 0x10;
				_hBlankHDMATransferCurrentOffset = 0x0000;
				
				_hdmaTransferActive = true;
				
				WriteByteToBank(0, address - IO_PORTS_OFFSET, SetBit(ReadByteFromBank(0, address - IO_PORTS_OFFSET), 7, false));
			}
			else if (!GetBit(value, 7) && _hdmaTransferActive)
			{
				_hdmaTransferActive = false;
				
				WriteByteToBank(0, address - IO_PORTS_OFFSET, SetBit(ReadByteFromBank(0, address - IO_PORTS_OFFSET), 7, true));
			}
			else
			{
				_hdmaMode = HDMAMode::GENERAL_PURPOSE_HDMA;
				
				ExecuteHDMATransfer();
			}
			
			break;
			
		case 0xFF56:
			// infrared communications port
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			break;
			
		case 0xFF68:
			// background palette index
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			if (GetBit(value, 7))
			{
				_colorBackgroundPaletteIndexAutoIncrement = true;
			}
			else
			{
				_colorBackgroundPaletteIndexAutoIncrement = false;
			}
			
			break;
			
		case 0xFF69:
			// background palette data
			// RECHECK THIS!!!
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			if ((((ReadByteFromBank(0, 0xFF68 - IO_PORTS_OFFSET) & 0x3F) % 8) % 2) == 0)
			{
				int palette = (ReadByteFromBank(0, 0xFF68 - IO_PORTS_OFFSET) & 0x3F) / 8;
				int color = ((ReadByteFromBank(0, 0xFF68 - IO_PORTS_OFFSET) & 0x3F) % 8) / 2;
				
				int newRed = value & 0x1F;
				int newGreen = (_colorBackgroundPalettes[palette][color].GetGreen() & 0x18) | ((value >> 5) & 0x07);
				
				_colorBackgroundPalettes[palette][color].SetRed(newRed);
				_colorBackgroundPalettes[palette][color].SetGreen(newGreen);
			}
			else
			{
				int palette = (ReadByteFromBank(0, 0xFF68 - IO_PORTS_OFFSET) & 0x3F) / 8;
				int color = ((ReadByteFromBank(0, 0xFF68 - IO_PORTS_OFFSET) & 0x3F) % 8) / 2;
				
				int newGreen = (_colorBackgroundPalettes[palette][color].GetGreen() & 0x07) | ((value << 3) & 0x18);
				int newBlue = (value >> 2) & 0x1F;
				
				_colorBackgroundPalettes[palette][color].SetGreen(newGreen);
				_colorBackgroundPalettes[palette][color].SetBlue(newBlue);
			}
			
			if (_colorBackgroundPaletteIndexAutoIncrement)
			{
				WriteByteToBank(0, 0xFF68 - IO_PORTS_OFFSET, (ReadByteFromBank(0, 0xFF68 - IO_PORTS_OFFSET) + 1) & 0xFF);
			}
			
			break;
			
		case 0xFF6A:
			// sprite palette index
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			if (GetBit(value, 7))
			{
				_colorSpritePaletteIndexAutoIncrement = true;
			}
			else
			{
				_colorSpritePaletteIndexAutoIncrement = false;
			}
			
			break;
			
		case 0xFF6B:
			// sprite palette data
			// RECHECK THIS!!!
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			if ((((ReadByteFromBank(0, 0xFF6A - IO_PORTS_OFFSET) & 0x3F) % 8) % 2) == 0)
			{
				int palette = (ReadByteFromBank(0, 0xFF6A - IO_PORTS_OFFSET) & 0x3F) / 8;
				int color = ((ReadByteFromBank(0, 0xFF6A - IO_PORTS_OFFSET) & 0x3F) % 8) / 2;
				
				int newRed = value & 0x1F;
				int newGreen = (_colorBackgroundPalettes[palette][color].GetGreen() & 0x18) | ((value >> 5) & 0x07);
				
				_colorSpritePalettes[palette][color].SetRed(newRed);
				_colorSpritePalettes[palette][color].SetGreen(newGreen);
			}
			else
			{
				int palette = (ReadByteFromBank(0, 0xFF6A - IO_PORTS_OFFSET) & 0x3F) / 8;
				int color = ((ReadByteFromBank(0, 0xFF6A - IO_PORTS_OFFSET) & 0x3F) % 8) / 2;
				
				int newGreen = (_colorBackgroundPalettes[palette][color].GetGreen() & 0x07) | ((value << 3) & 0x18);
				int newBlue = (value >> 2) & 0x1F;
				
				_colorBackgroundPalettes[palette][color].SetGreen(newGreen);
				_colorBackgroundPalettes[palette][color].SetBlue(newBlue);
			}
			
		case 0xFF70:
			// wram bank
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			if ((value & 0x07) == 0x00)
			{
				_selectedWorkRamBank = 0x01;
			}
			else
			{
				_selectedWorkRamBank = (value & 0x07);
			}
			
			break;
			
		default:
			WriteByteToBank(0, address - IO_PORTS_OFFSET, value);
			
			break;
	}
}

void IOPorts::Serialize(std::ostream &os)
{
	MemoryBlock::Serialize(os);
	
	// serialize more stuff
}

void IOPorts::Deserialize(std::istream &is)
{
	MemoryBlock::Deserialize(is);
	
	// deserialize more stuff
}

void IOPorts::ExecuteOAMDMATransfer()
{
	for (int i = 0; i < 0x0100; i++)
	{
		(*_bus).WriteByte(SpriteAttributeRam::SPRITE_ATTRIBUTE_RAM_OFFSET + i, (*_bus).ReadByte(_oamDMATransferAddress + i));
	}
}

void IOPorts::ExecuteHDMATransfer()
{
	if (_hdmaMode == HDMAMode::GENERAL_PURPOSE_HDMA)
	{
		for (int offset = 0; offset < _hdmaTransferLength; offset++)
		{
			WriteByte(_hdmaTransferDestinationAddress + offset, (*_bus).ReadByte(_hdmaTransferSourceAddress + offset));
		}
		
		// emulate bit 7 for reading, so the dma transfer "takes some time"
		// _rc.ioPorts[0xFF55 - 0xFF00] = SetBit(_rc.ioPorts[0xFF55 - 0xFF00], 7, GBC_TRUE);
		//_rc.ioPorts[address - 0xFF00] = 0xFF;
		_hdmaTransferActive = false;
	}
	else if (_hdmaMode == HDMAMode::HBLANK_HDMA)
	{
		for (int offset = _hBlankHDMATransferCurrentOffset; offset < _hBlankHDMATransferCurrentOffset + 0x10; offset++)
		{
			(*_bus).WriteByte(_hdmaTransferDestinationAddress + offset, (*_bus).ReadByte(_hdmaTransferSourceAddress + offset));
		}
		
		_hBlankHDMATransferCurrentOffset += 0x10;
		
		if (_hBlankHDMATransferCurrentOffset >= _hdmaTransferLength)
		{
			_hdmaTransferActive = false;
		}
	}
}

void IOPorts::SetMemoryBus(MemoryBus &bus)
{
	_bus = &bus;
}

void IOPorts::SetJoypad(Joypad &joypad)
{
	_joypad = &joypad;
}

void IOPorts::SetSerialPort(SerialPort &serialPort)
{
	_serialPort = &serialPort;
}

void IOPorts::SetHDMATransferActive(bool hdmaTransferActive)
{
	_hdmaTransferActive = hdmaTransferActive;
	
	WriteByteToBank(0, 0xFF55 - IO_PORTS_OFFSET, SetBit(ReadByteFromBank(0, 0xFF55 - IO_PORTS_OFFSET), 7, !_hdmaTransferActive));
}

void IOPorts::SetHDMAMode(HDMAMode hdmaMode)
{
	_hdmaMode = hdmaMode;
}

void IOPorts::SetHDMATransferSourceAddress(int hdmaTransferSourceAddress)
{
	_hdmaTransferSourceAddress = hdmaTransferSourceAddress;
	
	WriteByteToBank(0, 0xFF51 - IO_PORTS_OFFSET, GetHigh(hdmaTransferSourceAddress));
	WriteByteToBank(0, 0xFF52 - IO_PORTS_OFFSET, GetLow(hdmaTransferSourceAddress));
}

void IOPorts::SetHDMATransferDestinationAddress(int hdmaTransferDestinationAddress)
{
	_hdmaTransferDestinationAddress = hdmaTransferDestinationAddress;
	
	WriteByteToBank(0, 0xFF53 - IO_PORTS_OFFSET, GetHigh(hdmaTransferDestinationAddress));
	WriteByteToBank(0, 0xFF54 - IO_PORTS_OFFSET, GetLow(hdmaTransferDestinationAddress));
}

void IOPorts::SetHDMATransferLength(int transferLength)
{
	_hdmaTransferLength = transferLength;
	
	WriteByteToBank(0, 0xFF55 - IO_PORTS_OFFSET, (transferLength / 0x10) - 1);
}

void IOPorts::SetHBlankHDMATransferCurrentOffset(int hBlankHDMATransferCurrentOffset)
{
	_hBlankHDMATransferCurrentOffset = hBlankHDMATransferCurrentOffset;
}

void IOPorts::SetSelectedWorkRamBank(bool selectedWorkRamBank)
{
	_selectedWorkRamBank = selectedWorkRamBank;
}

void IOPorts::SetSelectedVideoRamBank(bool selectedVideoRamBank)
{
	_selectedVideoRamBank = selectedVideoRamBank;
}

void IOPorts::SetDirectionKeysSelected(bool directionKeysSelected)
{
	_directionKeysSelected = directionKeysSelected;
	
	WriteByteToBank(0, 0xFF00 - IO_PORTS_OFFSET, SetBit(ReadByteFromBank(0, 0xFF00 - IO_PORTS_OFFSET), 4, directionKeysSelected));
}

void IOPorts::SetButtonKeysSelected(bool buttonKeysSelected)
{
	_buttonKeysSelected = buttonKeysSelected;
	
	WriteByteToBank(0, 0xFF00 - IO_PORTS_OFFSET, SetBit(ReadByteFromBank(0, 0xFF00 - IO_PORTS_OFFSET), 5, buttonKeysSelected));
}

void IOPorts::SetLCDDisplayEnabled(bool lcdDisplayEnabled)
{
	_lcdDisplayEnabled = lcdDisplayEnabled;
	
	WriteByteToBank(0, 0xFF40 - IO_PORTS_OFFSET, SetBit(ReadByteFromBank(0, 0xFF40 - IO_PORTS_OFFSET), 7, lcdDisplayEnabled));
}

void IOPorts::SetWindowMapDisplaySelect(bool windowMapDisplaySelect)
{
	_windowMapDisplaySelect = windowMapDisplaySelect;
	
	WriteByteToBank(0, 0xFF40 - IO_PORTS_OFFSET, SetBit(ReadByteFromBank(0, 0xFF40 - IO_PORTS_OFFSET), 6, windowMapDisplaySelect));
}

void IOPorts::SetWindowDisplayEnabled(bool windowDisplayEnabled)
{
	_windowDisplayEnabled = windowDisplayEnabled;
	
	WriteByteToBank(0, 0xFF40 - IO_PORTS_OFFSET, SetBit(ReadByteFromBank(0, 0xFF40 - IO_PORTS_OFFSET), 5, windowDisplayEnabled));
}

void IOPorts::SetBackgroundAndWindowTileDataSelect(bool backgroundAndWindowTileDataSelect)
{
	_backgroundAndWindowTileDataSelect = backgroundAndWindowTileDataSelect;
	
	WriteByteToBank(0, 0xFF40 - IO_PORTS_OFFSET, SetBit(ReadByteFromBank(0, 0xFF40 - IO_PORTS_OFFSET), 4, backgroundAndWindowTileDataSelect));
}

void IOPorts::SetBackgroundMapDisplaySelect(bool backgroundMapDisplaySelect)
{
	_backgroundMapDisplaySelect = backgroundMapDisplaySelect;
	
	WriteByteToBank(0, 0xFF40 - IO_PORTS_OFFSET, SetBit(ReadByteFromBank(0, 0xFF40 - IO_PORTS_OFFSET), 3, backgroundMapDisplaySelect));
}

void IOPorts::SetSpriteSize(SpriteSize spriteSize)
{
	_spriteSize = spriteSize;
	
	WriteByteToBank(0, 0xFF40 - IO_PORTS_OFFSET, SetBit(ReadByteFromBank(0, 0xFF40 - IO_PORTS_OFFSET), 2, GetEnumValue(spriteSize)));
}

void IOPorts::SetSpriteDisplayEnabled(bool spriteDisplayEnabled)
{
	_spriteDisplayEnabled = spriteDisplayEnabled;
	
	WriteByteToBank(0, 0xFF40 - IO_PORTS_OFFSET, SetBit(ReadByteFromBank(0, 0xFF40 - IO_PORTS_OFFSET), 1, spriteDisplayEnabled));
}

void IOPorts::SetBackgroundDisplayEnabled(bool backgroundDisplayEnabled)
{
	_backgroundDisplayEnabled = backgroundDisplayEnabled;
	
	WriteByteToBank(0, 0xFF40 - IO_PORTS_OFFSET, SetBit(ReadByteFromBank(0, 0xFF40 - IO_PORTS_OFFSET), 0, backgroundDisplayEnabled));
}

void IOPorts::SetLCDMode(LCDMode lcdMode)
{
	_lcdMode = lcdMode;
	
	WriteByteToBank(0, 0xFF41 - IO_PORTS_OFFSET, (ReadByteFromBank(0, 0xFF41 - IO_PORTS_OFFSET) & 0xFC) | GetEnumValue(lcdMode) & 0x03);
}

void IOPorts::SetCoincidenceFlag(bool coincidenceFlag)
{
	_coincidenceFlag = coincidenceFlag;
	
	WriteByteToBank(0, 0xFF41 - IO_PORTS_OFFSET, SetBit(ReadByteFromBank(0, 0xFF41 - IO_PORTS_OFFSET), 2, coincidenceFlag));
}

void IOPorts::SetHBlankInterruptEnabled(bool hBlankInterruptEnabled)
{
	_hBlankInterruptEnabled = hBlankInterruptEnabled;
	
	WriteByteToBank(0, 0xFF41 - IO_PORTS_OFFSET, SetBit(ReadByteFromBank(0, 0xFF41 - IO_PORTS_OFFSET), 3, hBlankInterruptEnabled));
}

void IOPorts::SetVBlankInterruptEnabledInLCD(bool vBlankInterruptEnabledInLCD)
{
	_vBlankInterruptEnabledInLCD = vBlankInterruptEnabledInLCD;
	
	WriteByteToBank(0, 0xFF41 - IO_PORTS_OFFSET, SetBit(ReadByteFromBank(0, 0xFF41 - IO_PORTS_OFFSET), 4, vBlankInterruptEnabledInLCD));
}

void IOPorts::SetOAMInterruptEnabled(bool oamInterruptEnabled)
{
	_oamInterruptEnabled = oamInterruptEnabled;
	
	WriteByteToBank(0, 0xFF41 - IO_PORTS_OFFSET, SetBit(ReadByteFromBank(0, 0xFF41 - IO_PORTS_OFFSET), 5, oamInterruptEnabled));
}

void IOPorts::SetCoincidenceInterruptEnabled(bool coincidenceInterruptEnabled)
{
	_coincidenceInterruptEnabled = coincidenceInterruptEnabled;
	
	WriteByteToBank(0, 0xFF41 - IO_PORTS_OFFSET, SetBit(ReadByteFromBank(0, 0xFF41 - IO_PORTS_OFFSET), 6, coincidenceInterruptEnabled));
}

void IOPorts::SetScrollX(int scrollX)
{
	_scrollX = scrollX;
	
	WriteByteToBank(0, 0xFF43 - IO_PORTS_OFFSET, scrollX);
}

void IOPorts::SetScrollY(int scrollY)
{
	_scrollY = scrollY;
	
	WriteByteToBank(0, 0xFF42 - IO_PORTS_OFFSET, scrollY);
}

void IOPorts::SetLCDY(int lcdY)
{
	_lcdY = lcdY;
	
	WriteByteToBank(0, 0xFF44 - IO_PORTS_OFFSET, lcdY);
}

void IOPorts::SetLCDYCompare(int lcdYCompare)
{
	_lcdYCompare = lcdYCompare;
	
	WriteByteToBank(0, 0xFF45 - IO_PORTS_OFFSET, lcdYCompare);
}

void IOPorts::SetWindowX(int windowX)
{
	_windowX = windowX;
	
	WriteByteToBank(0, 0xFF4B - IO_PORTS_OFFSET, windowX);
}

void IOPorts::SetWindowY(int windowY)
{
	_windowY = windowY;
	
	WriteByteToBank(0, 0xFF4A - IO_PORTS_OFFSET, windowY);
}

void IOPorts::SetDivider(int divider)
{
	_divider = divider;
	
	WriteByteToBank(0, 0xFF04 - IO_PORTS_OFFSET, divider);
}

void IOPorts::SetTimerCounter(int timerCounter)
{
	_timerCounter = timerCounter;
	
	WriteByteToBank(0, 0xFF05 - IO_PORTS_OFFSET, timerCounter);
}

void IOPorts::SetTimerModulo(int timerModulo)
{
	_timerModulo = timerModulo;
	
	WriteByteToBank(0, 0xFF06 - IO_PORTS_OFFSET, timerModulo);
}

void IOPorts::SetTimerClockFrequency(int timerClockFrequency)
{
	_timerClockFrequency = timerClockFrequency;
	
	int bitValue;
	
	switch (timerClockFrequency)
	{
		case 1024:
			bitValue = 0;
			break;
			
		case 16:
			bitValue = 1;
			break;
			
		case 64:
			bitValue = 2;
			break;
			
		case 256:
			bitValue = 3;
			break;
			
		default:
			bitValue = 0;
			break;
	}
	
	WriteByteToBank(0, 0xFF07 - IO_PORTS_OFFSET, (ReadByteFromBank(0, 0xFF07 - IO_PORTS_OFFSET) & 0xFC) | bitValue);
}

void IOPorts::SetTimerStopped(bool timerStopped)
{
	_timerStopped = timerStopped;
	
	WriteByteToBank(0, 0xFF07 - IO_PORTS_OFFSET, SetBit(ReadByteFromBank(0, 0xFF07 - IO_PORTS_OFFSET), 2, timerStopped));
}

MemoryBus &IOPorts::GetMemoryBus()
{
	return (*_bus);
}

Joypad &IOPorts::GetJoypad()
{
	return (*_joypad);
}

SerialPort &IOPorts::GetSerialPort()
{
	return (*_serialPort);
}

bool IOPorts::GetRightPressed()
{
	return (*_joypad).GetRight();
}

bool IOPorts::GetLeftPressed()
{
	return (*_joypad).GetLeft();
}

bool IOPorts::GetUpPressed()
{
	return (*_joypad).GetUp();
}

bool IOPorts::GetDownPressed()
{
	return (*_joypad).GetDown();
}

bool IOPorts::GetButtonAPressed()
{
	return (*_joypad).GetButtonA();
}

bool IOPorts::GetButtonBPressed()
{
	return (*_joypad).GetButtonB();
}

bool IOPorts::GetSelectPressed()
{
	return (*_joypad).GetSelect();
}

bool IOPorts::GetStartPressed()
{
	return (*_joypad).GetStart();
}

bool IOPorts::GetHDMATransferActive()
{
	return _hdmaTransferActive;
}

HDMAMode IOPorts::GetHDMAMode()
{
	return _hdmaMode;
}

int IOPorts::GetHDMATransferSourceAddress()
{
	return _hdmaTransferSourceAddress;
}

int IOPorts::GetHDMATransferDestinationAddress()
{
	return _hdmaTransferDestinationAddress;
}

int IOPorts::GetHDMATransferLength()
{
	return _hdmaTransferLength;
}

int IOPorts::GetHBlankHDMATransferCurrentOffset()
{
	return _hBlankHDMATransferCurrentOffset;
}

bool IOPorts::GetSelectedWorkRamBank()
{
	return _selectedWorkRamBank;
}

bool IOPorts::GetSelectedVideoRamBank()
{
	return _selectedVideoRamBank;
}

bool IOPorts::GetDirectionKeysSelected()
{
	return _directionKeysSelected;
}

bool IOPorts::GetButtonKeysSelected()
{
	return _buttonKeysSelected;
}

bool IOPorts::GetLCDDisplayEnabled()
{
	return _lcdDisplayEnabled;
}

bool IOPorts::GetWindowMapDisplaySelect()
{
	return _windowMapDisplaySelect;
}

bool IOPorts::GetWindowDisplayEnabled()
{
	return _windowDisplayEnabled;
}

bool IOPorts::GetBackgroundAndWindowTileDataSelect()
{
	return _backgroundAndWindowTileDataSelect;
}

bool IOPorts::GetBackgroundMapDisplaySelect()
{
	return _backgroundMapDisplaySelect;
}

SpriteSize IOPorts::GetSpriteSize()
{
	return _spriteSize;
}

bool IOPorts::GetSpriteDisplayEnabled()
{
	return _spriteDisplayEnabled;
}

bool IOPorts::GetBackgroundDisplayEnabled()
{
	return _backgroundDisplayEnabled;
}

LCDMode IOPorts::GetLCDMode()
{
	return _lcdMode;
}

bool IOPorts::GetCoincidenceFlag()
{
	return _coincidenceFlag;
}

bool IOPorts::GetHBlankInterruptEnabled()
{
	return _hBlankInterruptEnabled;
}

bool IOPorts::GetVBlankInterruptEnabledInLCD()
{
	return _vBlankInterruptEnabledInLCD;
}

bool IOPorts::GetOAMInterruptEnabled()
{
	return _oamInterruptEnabled;
}

bool IOPorts::GetCoincidenceInterruptEnabled()
{
	return _coincidenceInterruptEnabled;
}

int IOPorts::GetScrollX()
{
	return _scrollX;
}

int IOPorts::GetScrollY()
{
	return _scrollY;
}

int IOPorts::GetLCDY()
{
	return _lcdY;
}

int IOPorts::GetLCDYCompare()
{
	return _lcdYCompare;
}

int IOPorts::GetWindowX()
{
	return _windowX;
}

int IOPorts::GetWindowY()
{
	return _windowY;
}

int IOPorts::GetDivider()
{
	return _divider;
}

int IOPorts::GetTimerCounter()
{
	return _timerCounter;
}

int IOPorts::GetTimerModulo()
{
	return _timerModulo;
}

int IOPorts::GetTimerClockFrequency()
{
	return _timerClockFrequency;
}

bool IOPorts::GetTimerStopped()
{
	return _timerStopped;
}

Array<Color<int>, 4> &IOPorts::GetMonochromeBackgroundPalette()
{
	return _monochromeBackgroundPalette;
}

Array<Color<int>, 4> &IOPorts::GetMonochromeSpritePalette(int paletteNumber)
{
	return _monochromeSpritePalettes[paletteNumber];
}

Array<Color<int>, 4> &IOPorts::GetColorBackgroundPalette(int paletteNumber)
{
	return _colorBackgroundPalettes[paletteNumber];
}

Array<Color<int>, 4> &IOPorts::GetColorSpritePalette(int paletteNumber)
{
	return _colorSpritePalettes[paletteNumber];
}
