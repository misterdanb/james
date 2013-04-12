#include "RomRam.hpp"

gbc::core::cartridges::RomRam::RomRam(int rom[], int size)
	: Cartridge(rom, size)
{
}

gbc::core::cartridges::RomRam::~RomRam()
{
}

int gbc::core::cartridges::RomRam::ReadByte(int address)
{
	return 0;
}

void gbc::core::cartridges::RomRam::WriteByte(int address, int value)
{
}
