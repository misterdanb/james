#include "MBC5.hpp"

using namespace gbc;
using namespace gbc::core;
using namespace gbc::core::cartridges;

MBC5::MBC5(DynamicArray<int> rom)
	: Cartridge(rom)
{
}

MBC5::~MBC5()
{
}

int MBC5::ReadByte(int address)
{
	return 0;
}

void MBC5::WriteByte(int address, int value)
{
}
