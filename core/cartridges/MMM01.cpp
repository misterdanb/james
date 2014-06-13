#include "MMM01.hpp"

using namespace james;
using namespace james::core;
using namespace james::core::cartridges;

MMM01::MMM01 (DynamicArray<int> rom)
	: Cartridge (rom)
{
}

MMM01::~MMM01()
{
}

int MMM01::ReadByte (int)
{
	return 0;
}

void MMM01::WriteByte (int, int)
{
	return;
}
