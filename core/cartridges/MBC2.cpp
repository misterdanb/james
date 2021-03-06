#include "MBC2.hpp"

using namespace james;
using namespace james::core;
using namespace james::core::cartridges;

MBC2::MBC2 (DynamicArray<int> rom)
	: Cartridge (rom), _ramEnabled (GBC_FALSE)
{
}

MBC2::~MBC2()
{
}

int MBC2::ReadByte (int address)
{
	if (address >= 0x0000 && address <= 0x3FFF)
	{
		return _rom[address];
	}
	else if (address >= 0x4000 && address <= 0x7FFF)
	{
		return _rom[_selectedRomBank * _header.romDimensions.bankSize + address];
	}
	else if (address >= 0xA000 && address <= 0xA1FF)
	{
		if (_ramEnabled)
		{
			return (_ram[_selectedRamBank * _header.ramDimensions.bankSize + address] >> ((address % 2) * 4)) & 0x0F;
		}
		else
		{
			return 0x00;
		}
	}

	ERROR ("MBC2: Address out of range!");

	return 0x00;
}

void MBC2::WriteByte (int address, int value)
{
	if (address >= 0x0000 && address <= 0x1FFF)
	{
		_ramEnabled = (value & 0x0F) == 0x0A;
	}
	else if (address >= 0x2000 && address <= 0x3FFF)
	{
		if (value == 0x00)
		{
			_selectedRomBank = 0x01;
		}
		else
		{
			_selectedRomBank &= 0xE0;
			_selectedRomBank |= (value & 0x1F);
		}
	}
	else if (address >= 0xA000 && address <= 0xA1FF)
	{
		if (_ramEnabled)
		{
			_ram[_selectedRamBank * _header.ramDimensions.bankSize + address] &= 0x0F << ((address % 2) * 4);
			_ram[_selectedRamBank * _header.ramDimensions.bankSize + address] |= (value & 0x0F) << ((address % 2) * 4);
		}
	}
	else
	{
		ERROR ("MBC2: Address out of range!");
	}
}
