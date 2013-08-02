#include "MBC3.hpp"

using namespace gbc;
using namespace gbc::core;
using namespace gbc::core::cartridges;

MBC3::MBC3(DynamicArray<int> rom)
	: Cartridge(rom), _ramRtcEnabled(GBC_FALSE), _ramRtcMode(0x00), _rtc()
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
		return _rom[_selectedRomBank * _header.romDimensions.bankSize + address - 0x4000];
	}
	else if (address >= 0xA000 && address <= 0xBFFF)
	{
		if (_ramRtcEnabled)
		{
			if (_ramRtcMode >= 0x00 && _ramRtcMode <= 0x03)
			{
				return _ram[_selectedRamBank * _header.ramDimensions.bankSize + address - 0xA000];
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
		_ramRtcEnabled = (value & 0x0F) == 0x0A;
	}
	else if (address >= 0x2000 && address <= 0x3FFF)
	{
		_selectedRomBank = (value & 0x7F);
		
		if (_selectedRomBank == 0x00)
		{
			_selectedRomBank = 0x01;
		}
		
		_selectedRomBank %= _header.romDimensions.banks;
	}
	else if (address >= 0x4000 && address <= 0x5FFF)
	{
		if (value >= 0x00 && value <= 0x03)
		{
			_selectedRamBank = value & 0xFF;
			_selectedRamBank %= _header.ramDimensions.banks;
			
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
			if (_ramRtcMode >= 0x00 && _ramRtcMode <= 0x03)
			{
				_ram[_selectedRamBank * _header.ramDimensions.bankSize + address - 0xA000] = value;
			}
			else if (_ramRtcMode >= 0x08 && _ramRtcMode <= 0x0C)
			{
				// rtc register thing
			}
		}
	}
	else
	{
		ERROR("MBC3: Address out of range!");
	}
}
