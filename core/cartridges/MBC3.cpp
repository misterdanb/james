#include "MBC3.hpp"

using namespace gbc;
using namespace gbc::core;
using namespace gbc::core::cartridges;

MBC3::MBC3(DynamicArray<int> rom)
	: Cartridge(rom), _ramRtcEnabled(GBC_FALSE), _ramRtcMode(0x00)
{
}

MBC3::~MBC3()
{
}

int MBC3::ReadByte(int address)
{
	if (address >= 0x0000 && address <= 0x3FFF)
	{
		return _rom[address];
	}
	else if (address >= 0x4000 && address <= 0x7FFF)
	{
		return _rom[_selectedRomBank * _header.romDimensions.bankSize + address];
	}
	else if (address >= 0xA000 && address <= 0xBFFF)
	{
		if (_ramRtcEnabled)
		{
			if (_ramRtcMode >= 0x00 && _ramRtcMode <= 0x03)
			{
				return _ram[_selectedRamBank * _header.ramDimensions.bankSize + address];
			}
			else if (_ramRtcMode >= 0x08 && _ramRtcMode <= 0x0C)
			{
				return _rtc[_ramRtcMode - 0x08];
			}
		}
		else
		{
			return 0x00;
		}
	}
	
	ERROR("MBC3: Address out of range!");
	
	return 0x00;
}

void MBC3::WriteByte(int address, int value)
{
	if (address >= 0x0000 && address <= 0x1FFF)
	{
		_ramRtcEnabled = GBC_FALSE; // not sure what to implement here oO
	}
	else if (address >= 0x2000 && address <= 0x3FFF)
	{
		if (value == 0x00)
		{
			_selectedRomBank = 0x01;
		}
		else
		{
			_selectedRomBank &= 0x80;
			_selectedRomBank |= (value & 0x7F);
		}
	}
	else if (address >= 0x4000 && address <= 0x5FFF)
	{
		if (value >= 0x00 && value <= 0x03)
		{
			_selectedRamBank = value & 0xFF;
			_ramRtcMode = value;
		}
		else if (value >= 0x08 && value <= 0x0C)
		{
			_ramRtcMode = value;
		}
	}
	else if (address >= 0x6000 && address <= 0x7FFF)
	{
		// todo: latch current time or so
	}
	else if (address >= 0xA000 && address <= 0xBFFFF)
	{
		if (_ramRtcEnabled)
		{
			_ram[_selectedRamBank * _header.ramDimensions.bankSize + address] = value;
		}
	}
	else
	{
		ERROR("MBC3: Address out of range!");
	}
}
