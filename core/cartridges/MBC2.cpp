#include "MBC2.hpp"

gbc::core::cartridges::MBC2::MBC2(int rom[])
	: Cartridge(rom)
{
}

gbc::core::cartridges::MBC2::~MBC2()
{
}

int gbc::core::cartridges::MBC2::ReadByte(int address)
{
	return 0;
}

void gbc::core::cartridges::MBC2::WriteByte(int address, int value)
{
}
