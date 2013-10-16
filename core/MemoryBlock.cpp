#include "MemoryBlock.hpp"

using namespace james;
using namespace james::core;

MemoryBlock::MemoryBlock(int banks, int size)
	: _banks(banks), _size(size), _data()
{
	_data.resize(banks);
	
	for (DynamicArray<int> bank : _data)
	{
		bank.resize(size);
	}
}

MemoryBlock::~MemoryBlock()
{
}

int MemoryBlock::ReadByteFromBank(int bank, int address)
{
	return _data[bank][address];
}

void MemoryBlock::WriteByteToBank(int bank, int address, int value)
{
	_data[bank][address] = value;
}

void MemoryBlock::Serialize(std::ostream &os)
{
	for (DynamicArray<int> &bank : _data)
	{
		DynamicArray<char> outBank(bank.begin(), bank.end());
		
		os.write(&outBank[0], _size);
	}
}

void MemoryBlock::Deserialize(std::istream &is)
{
	for (DynamicArray<int> &bank : _data)
	{
		DynamicArray<char> inBank;
		
		inBank.resize(_size);
		
		is.read(&inBank[0], _size);
		
		// could be problematic
		bank = DynamicArray<int>(inBank.begin(), inBank.end());
	}
}

void MemoryBlock::Reset()
{
	for (DynamicArray<int> &bank : _data)
	{
		std::fill(bank.begin(), bank.end(), 0x00);
	}
}
