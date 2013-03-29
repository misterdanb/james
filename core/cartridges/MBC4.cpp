#include "MBC4.hpp"

gbc::core::cartridges::MBC4::MBC4(int rom[])
	: Cartridge(rom)
{
}

gbc::core::cartridges::MBC4::~MBC4()
{
}

int gbc::core::cartridges::MBC4::ReadByte(int address)
{
	return 0;
}

void gbc::core::cartridges::MBC4::WriteByte(int address, int value)
{
}
