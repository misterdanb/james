#include "RomRam.hpp"

using namespace gbc;
using namespace gbc::core;
using namespace gbc::core::cartridges;

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
