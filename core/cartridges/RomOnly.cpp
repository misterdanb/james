#include "RomOnly.hpp"

gbc::core::cartridges::RomOnly::RomOnly(int rom[])
	: Cartridge(rom)
{
}

gbc::core::cartridges::RomOnly::~RomOnly()
{
}

int gbc::core::cartridges::RomOnly::ReadByte(int address)
{
	return 0;
}

void gbc::core::cartridges::RomOnly::WriteByte(int address, int value)
{
}
