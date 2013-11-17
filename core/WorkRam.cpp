#include "WorkRam.hpp"

using namespace james;
using namespace james::core;

WorkRam::WorkRam()
	: MemoryBlock(WORK_RAM_BANKS, WORK_RAM_BANK_SIZE), _ioPorts(nullptr)
{
}

WorkRam::~WorkRam()
{
}

int WorkRam::ReadByte(int address)
{
	if (address < WORK_RAM_OFFSET_1 + WORK_RAM_BANK_SIZE)
	{
		return ReadByteFromBank(0, address - WORK_RAM_OFFSET_1);
	}
	else if (address < WORK_RAM_OFFSET_2 + WORK_RAM_BANK_SIZE)
	{
		return ReadByteFromBank((*_ioPorts).GetSelectedWorkRamBank(), address - WORK_RAM_OFFSET_2);
	}
	else if (address < WORK_RAM_OFFSET_3 + WORK_RAM_BANK_SIZE)
	{
		return ReadByteFromBank(0, address - WORK_RAM_OFFSET_1);
	}
	else if (address < WORK_RAM_OFFSET_4 + WORK_RAM_BANK_SIZE)
	{
		return ReadByteFromBank((*_ioPorts).GetSelectedWorkRamBank(), address - WORK_RAM_OFFSET_3);
	}
	
	return 0x00;
}

void WorkRam::WriteByte(int address, int value)
{
	if (address < WORK_RAM_OFFSET_1 + WORK_RAM_BANK_SIZE)
	{
		WriteByteToBank(0, address - WORK_RAM_OFFSET_1, value);
	}
	else if (address < WORK_RAM_OFFSET_2 + WORK_RAM_BANK_SIZE)
	{
		WriteByteToBank((*_ioPorts).GetSelectedWorkRamBank(), address - WORK_RAM_OFFSET_2, value);
	}
	else if (address < WORK_RAM_OFFSET_3 + WORK_RAM_BANK_SIZE)
	{
		WriteByteToBank(0, address - WORK_RAM_OFFSET_1, value);
	}
	else if (address < WORK_RAM_OFFSET_4 + WORK_RAM_BANK_SIZE)
	{
		WriteByteToBank((*_ioPorts).GetSelectedWorkRamBank(), address - WORK_RAM_OFFSET_3, value);
	}
}

void WorkRam::SetIOPorts(IOPorts &ioPorts)
{
	_ioPorts = &ioPorts;
}

IOPorts &WorkRam::GetIOPorts()
{
	return *_ioPorts;
}
