#include "MemoryBus.hpp"

j16 MemoryBus::ReadWord (j16 address)
{
	return JoinBytes (ReadByte (address + 1), ReadByte (address));
}

void MemoryBus::WriteWord (j16 address, j16 value)
{
	WriteByte (address, GetLow (value));
	WriteByte (address + 1, GetHigh (value));
}