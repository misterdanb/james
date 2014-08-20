#include "MemoryBlock.hpp"

using namespace james;
using namespace james::core;

MemoryBlock::MemoryBlock (int amountOfBanks, int bankSize)
	: _amountOfBanks (amountOfBanks),
	  _bankSize (bankSize),
	  _activeBankNumber (0),
	  _banks (amountOfBanks, std::vector<int> (bankSize, 0x00))
{
	_banks.resize (amountOfBanks);

	for (DynamicArray<int> bank : _banks)
	{
		bank.resize (bankSize, 0x00);
	}
}

int MemoryBlock::ReadByteFromBank (int bankNumber, int address) const
{
	return _banks[bankNumber][address];
}

void MemoryBlock::WriteByteToBank (int bankNumber, int address, int value)
{
	_banks[bankNumber][address] = value;
}

int MemoryBlock::ReadByte (int address)
{
	return _banks[_activeBankNumber][address];
}

void MemoryBlock::WriteByte (int address, int value)
{
	_banks[_activeBankNumber][address] = value;
}

void MemoryBlock::Serialize (std::ostream& os)
{
	for (DynamicArray<int>& bank : _banks)
	{
		DynamicArray<char> outBank (bank.begin(), bank.end());

		os.write (&outBank[0], _bankSize);
	}
}

void MemoryBlock::Deserialize (std::istream& is)
{
	for (DynamicArray<int>& bank : _banks)
	{
		DynamicArray<char> inBank;

		inBank.resize (_bankSize);

		is.read (&inBank[0], _bankSize);

		// could be problematic
		bank = DynamicArray<int> (inBank.begin(), inBank.end());
	}
}

void MemoryBlock::Reset()
{
	for (DynamicArray<int>& bank : _banks)
	{
		std::fill (bank.begin(), bank.end(), 0x00);
	}
}

void MemoryBlock::SetActiveBank (int bankNumber)
{
	_activeBankNumber = bankNumber;
}

int MemoryBlock::GetActiveBank() const
{
	return _activeBankNumber;
}
