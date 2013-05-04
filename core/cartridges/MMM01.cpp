#include "MMM01.hpp"

gbc::core::cartridges::MMM01::MMM01(DynamicArray<int> rom)
	: Cartridge(rom)
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
