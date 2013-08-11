#include "RomRam.hpp"

using namespace james;
using namespace james::core;
using namespace james::core::cartridges;

RomRam::RomRam(DynamicArray<int> rom)
	: Cartridge(rom)
{
}

RomRam::~RomRam()
{
}

int RomRam::ReadByte(int address)
{
	return 0;
}

void RomRam::WriteByte(int address, int value)
{
}
