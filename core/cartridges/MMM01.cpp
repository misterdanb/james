#include "MMM01.hpp"

gbc::core::cartridges::MMM01::MMM01(int rom[], int size)
	: Cartridge(rom, size)
{
}

gbc::core::cartridges::MMM01::~MMM01()
{
}

int gbc::core::cartridges::MMM01::ReadByte(int address)
{
	return 0;
}

void gbc::core::cartridges::MMM01::WriteByte(int address, int value)
{
}
