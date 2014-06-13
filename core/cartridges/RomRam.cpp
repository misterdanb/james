#include "RomRam.hpp"

using namespace james;
using namespace james::core;
using namespace james::core::cartridges;

RomRam::RomRam (DynamicArray<int> rom)
	: Cartridge (rom)
{
}

RomRam::~RomRam()
{
}

int RomRam::ReadByte (int)
{
	return 0;
}

void RomRam::WriteByte (int, int)
{
	return;
}
