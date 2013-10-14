#include "HighRam.hpp"

using namespace james;
using namespace james::core;

HighRam::HighRam()
	: MemoryBlock(HIGH_RAM_BANKS, HIGH_RAM_BANK_SIZE)
{
}

HighRam::~HighRam()
{
}

int HighRam::ReadByte(int address)
{
	return ReadByteFromBank(0, address);
}

void HighRam::WriteByte(int address, int value)
{
	WriteByteToBank(0, address, value);
}
