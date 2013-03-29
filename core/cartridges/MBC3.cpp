#include "MBC3.hpp"

gbc::core::cartridges::MBC3::MBC3(int rom[])
	: Cartridge(rom)
{
}

gbc::core::cartridges::MBC3::~MBC3()
{
}

int gbc::core::cartridges::MBC3::ReadByte(int address)
{
	return 0;
}

void gbc::core::cartridges::MBC3::WriteByte(int address, int value)
{
}
