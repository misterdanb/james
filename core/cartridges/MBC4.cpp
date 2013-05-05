#include "MBC4.hpp"

using namespace gbc;
using namespace gbc::core;
using namespace gbc::core::cartridges;

MBC4::MBC4(DynamicArray<int> rom)
	: Cartridge(rom)
{
}

MBC4::~MBC4()
{
}

int MBC4::ReadByte(int address)
{
	return 0;
}

void MBC4::WriteByte(int address, int value)
{
}
