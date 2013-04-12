#include "MBC5.hpp"

gbc::core::cartridges::MBC5::MBC5(int rom[], int size)
	: Cartridge(rom, size)
{
}

gbc::core::cartridges::MBC5::~MBC5()
{
}

int gbc::core::cartridges::MBC5::ReadByte(int address)
{
	return 0;
}

void gbc::core::cartridges::MBC5::WriteByte(int address, int value)
{
}
