#include "MBC4.hpp"

using namespace james;
using namespace james::core;
using namespace james::core::cartridges;

MBC4::MBC4(DynamicArray<int> rom)
	: Cartridge(rom)
{
}

MBC4::~MBC4()
{
}

int MBC4::ReadByte(int)
{
	return 0;
}

void MBC4::WriteByte(int, int)
{
	return;
}
