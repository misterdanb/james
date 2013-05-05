#include "MMM01.hpp"

using namespace gbc;
using namespace gbc::core;
using namespace gbc::core::cartridges;

MMM01::MMM01(DynamicArray<int> rom)
	: Cartridge(rom)
{
}

MMM01::~MMM01()
{
}

int MMM01::ReadByte(int address)
{
	return 0;
}

void MMM01::WriteByte(int address, int value)
{
}
