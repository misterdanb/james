#include "RomRam.hpp"

gbc::core::cartridges::RomRam::RomRam(int rom[])
	: Cartridge(rom)
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
