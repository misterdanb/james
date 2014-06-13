#include "RomOnly.hpp"

using namespace james;
using namespace james::core;
using namespace james::core::cartridges;

RomOnly::RomOnly (DynamicArray<int> rom)
	: Cartridge (rom)
{
}

RomOnly::~RomOnly()
{
}

int RomOnly::ReadByte (int address)
{
	if (address >= 0x0000 && address <= 0x7FFF)
	{
		return _rom[address];
	}
	else if (address >= 0xA000 && address <= 0xBFFFF)
	{
		return _ram[address  - 0xA000];
	}

	ERROR ("RomOnly: Address out of range!");

	return 0x00;
}

void RomOnly::WriteByte (int address, int value)
{
	if (address >= 0x0000 && address <= 0x7FFF)
	{
		return;
	}
	else if (address >= 0xA000 && address <= 0xBFFFF)
	{
		_ram[address - 0xA000] = value & 0xFF;
	}
	else
	{
		ERROR ("RomOnly: Address out of range!");
	}
}
