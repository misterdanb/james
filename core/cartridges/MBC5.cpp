#include "MBC5.hpp"

using namespace gbc;
using namespace gbc::core;
using namespace gbc::core::cartridges;

MBC5::MBC5(DynamicArray<int> rom)
	: Cartridge(rom),
	  _ramEnabled(),
	  _romRamMode()
{
}

MBC5::~MBC5()
{
}

int MBC5::ReadByte(int address)
{
	if (address <= 0x3FFF) 
	{
		return _rom[address];
	}
	else if (address <= 0x7FFF)
	{
		return _rom[_selectedRomBank * _header.romDimensions.bankSize + address - 0x4000];
	}
	else if (address <= 0xBFFF)
	{
		if (_ramEnabled)
		{
			return _ram[_selectedRamBank * _header.ramDimensions.bankSize + address - 0xA000];
		}
		else
		{
			return 0x00;
		}
	}
	
	ERROR("MBC5: Address out of range!");
	
	return 0x00;
}

void MBC5::WriteByte(int address, int value)
{
	if (address <= 0x1FFF)
	{
		_ramEnabled = (value & 0x0F) == 0x0A;
	}
	else if (address <= 0x2FFF)
	{
		_selectedRomBank &= 0x0100;
		_selectedRomBank = (value & 0xFF);
		
		if (_selectedRomBank == 0x20)
		{
			_selectedRomBank = 0x21;
		}
		else if (_selectedRomBank == 0x40)
		{
			_selectedRomBank = 0x41;
		}
		else if (_selectedRomBank == 0x60) 
		{
			_selectedRomBank = 0x61;
		}
	}
	else if (address <= 0x3FFF)
	{
		_selectedRomBank &= 0xFF;
		_selectedRomBank = (value & 0x01) << 8;
	}
	else if (address <= 0x5FFF)
	{
		_selectedRamBank = (value & 0x0F);
	}
	else if (address <= 0x7FFF)
	{
	}
	else if (address <= 0xBFFF)
	{
		if (address - 0xA000 < _header.ramDimensions.bankSize)
		{
			if (_ramEnabled)
			{
				_ram[_selectedRamBank * _header.ramDimensions.bankSize +  address - 0xA000] = value;
			}
		}
	}
	else
	{
		ERROR("MBC5: Address out of range!");
	}
}
