#include "MBC1.hpp"

using namespace gbc;
using namespace gbc::core;
using namespace gbc::core::cartridges;

MBC1::MBC1(DynamicArray<int> rom)
	: Cartridge(rom), _ramEnabled(GBC_FALSE), _romRamMode(0x00)
{
}

MBC1::~MBC1()
{
}

int MBC1::ReadByte(int address)
{
	if (address <= 0x3FFF) 
	{
		return _rom[address];
	}
	else if (address <= 0x7FFF)
	{
		return _rom[_selectedRomBank * _header.romDimensions.bankSize + address - 0x4000];
	}
	else if (address <= 0xBFFF)
	{
		if (_ramEnabled)
		{
			return _ram[_selectedRamBank * _header.ramDimensions.bankSize + address - 0xA000];
		}
		else
		{
			return 0x00;
		}
	}
	
	ERROR("MBC1: Address out of range!");
	
	return 0x00;
}

void MBC1::WriteByte(int address, int value)
{
	if (address <= 0x1FFF)
	{
		_ramEnabled = (value & 0x0F) == 0x0A;
	}
	else if (address <= 0x3FFF)
	{
		if (value == 0x00)
		{
			_selectedRomBank = 0x01;
		}
		else
		{
			_selectedRomBank &= 0xE0;
			_selectedRomBank |= (value & 0x1F);
		}
		
		//if (_romBank >= _header.RomBanks) _romBank = _header.RomBanks - 1;
	}
	else if (address <= 0x5FFF)
	{
		if (_romRamMode == 0x00)
		{
			_selectedRomBank &= 0x1F;
			_selectedRomBank |= (value << 5) & 0x60;
			
			if (_selectedRomBank == 0x20)
			{
				_selectedRomBank = 0x21;
			}
			else if (_selectedRomBank == 0x40)
			{
				_selectedRomBank = 0x41;
			}
			else if (_selectedRomBank == 0x60) 
			{
				_selectedRomBank = 0x61;
			}
		}
		else if (_romRamMode == 0x01)
		{
			_selectedRamBank = value & 0x03;
		}
	}
	else if (address <= 0x7FFF)
	{
		_romRamMode = value & 0x01;
		
		if (_romRamMode == 0x00)
		{
			_selectedRamBank = 0x00;
		}
		else if (_romRamMode == 0x01)
		{
			_selectedRomBank &= 0x1F;
		}
	}
	else if (address <= 0xBFFF)
	{
		if (address - 0xA000 < _header.ramDimensions.bankSize)
		{
			if (_ramEnabled)
			{
				_ram[_selectedRamBank * _header.ramDimensions.bankSize +  address - 0xA000] = value;
			}
		}
	}
	else
	{
		ERROR("MBC1: Address out of range!");
	}
}
