#include "MBC1.hpp"

gbc::core::cartridges::MBC1::MBC1(int rom[])
	: Cartridge(rom)
{
}

gbc::core::cartridges::MBC1::~MBC1()
{
}

int gbc::core::cartridges::MBC1::ReadByte(int address)
{
	return 0;
}

void gbc::core::cartridges::MBC1::WriteByte(int address, int value)
{
}
