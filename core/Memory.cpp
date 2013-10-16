#include "Memory.hpp"

using namespace james;
using namespace james::core;

Memory::Memory()
{
}

Memory::~Memory()
{
}

int Memory::ReadByte(int address)
{
	if (address <= 0x7FFF)
	{
		return _cartridge->ReadByte(address);
	}
	else if (address <= 0x9FFF)
	{
		return _videoRam->ReadByte(address);
	}
	else if (address <= 0xBFFF)
	{
		return _cartridge->ReadByte(address);
	}
	else if (address <= 0xFDFF)
	{
		return _workRam->ReadByte(address);
	}
	else if (address <= 0xFE9F)
	{
		return _spriteAttributeRam->ReadByte(address);
	}
	else if (address <= 0xFEFF)
	{
		return 0x00;
	}
	else if (address <= 0xFF7F)
	{
		return _ioPorts->ReadByte(address);
	}
	else if (address <= 0xFFFE)
	{
		return _highRam->ReadByte(address);
	}
	else if (address == InterruptHandler::INTERRUPT_ENABLE_ADDRESS)
	{
		return _interruptEnableRegister;
	}
	
	LOG(std::string("Memory: Address out of range: ") + ToHex(address));
	
	return 0x00;
}

void Memory::WriteByte(int address, int value)
{
	if (address >= 0x0000 && address <= 0x7FFF)
	{
		_cartridge->WriteByte(address, value);
	}
	else if (address <= 0x9FFF)
	{
		if ((*_ioPorts).GetLCDMode() != LCDMode::TRANSFERRING_DATA)
		{
			_videoRam->WriteByte(address, value);
		}
	}
	else if (address <= 0xBFFF)
	{
		_cartridge->WriteByte(address, value);
	}
	else if (address <= 0xFDFF)
	{
		_workRam->WriteByte(address, value);
	}
	else if (address <= 0xFE9F)
	{
		if ((*_ioPorts).GetLCDMode() != LCDMode::SEARCHING_OAM &&
		    (*_ioPorts).GetLCDMode() != LCDMode::TRANSFERRING_DATA)
		{
			_spriteAttributeRam->WriteByte(address, value);
		}
	}
	else if (address <= 0xFEFF)
	{
	}
	else if (address <= 0xFF7F)
	{
		_ioPorts->WriteByte(address, value);
	}
	else if (address <= 0xFFFE)
	{
		_highRam->WriteByte(address, value);
	}
	else if (address == InterruptHandler::INTERRUPT_ENABLE_ADDRESS)
	{
		_interruptEnableRegister = value;
	}
	else
	{
		ERROR(std::string("Memory: Address out of range: ") + ToHex(address));
	}
}

void Memory::Serialize(std::ostream &os)
{
	os << (*_cartridge)
	   << (*_videoRam)
	   << (*_workRam)
	   << (*_spriteAttributeRam)
	   << (*_ioPorts)
	   << (*_highRam);
	
	char outInterruptEnableRegister = char(GetInterruptEnableRegister());
	
	os.write(&outInterruptEnableRegister, 1);
}

void Memory::Deserialize(std::istream &is)
{
	is >> (*_cartridge)
	   >> (*_videoRam)
	   >> (*_spriteAttributeRam)
	   >> (*_ioPorts)
	   >> (*_highRam);
	
	char inInterruptEnableRegister;
	
	is.read(&inInterruptEnableRegister, 1);
	
	SetInterruptEnableRegister(int(inInterruptEnableRegister));
}

void Memory::SetCartridge(cartridges::Cartridge &cartridge)
{
	_cartridge = &cartridge;
}

void Memory::SetVideoRam(VideoRam &videoRam)
{
	_videoRam = &videoRam;
}

void Memory::SetWorkRam(WorkRam &workRam)
{
	_workRam = &workRam;
}

void Memory::SetSpriteAttributeRam(SpriteAttributeRam &spriteAttributeRam)
{
	_spriteAttributeRam = &spriteAttributeRam;
}

void Memory::SetIOPorts(IOPorts &ioPorts)
{
	_ioPorts = &ioPorts;
}

void Memory::SetHighRam(HighRam &highRam)
{
	_highRam = &highRam;
}

void Memory::SetVBlankInterruptEnabled(bool vBlankInterruptEnabled)
{
	_interruptEnableRegister = SetBit(_interruptEnableRegister, InterruptHandler::VBLANK_INTERRUPT_BIT_NUMBER, vBlankInterruptEnabled);
}

void Memory::SetLCDStatusInterruptEnabled(bool lcdStatusInterruptEnabled)
{
	_interruptEnableRegister = SetBit(_interruptEnableRegister, InterruptHandler::LCD_STATUS_INTERRUPT_BIT_NUMBER, lcdStatusInterruptEnabled);
}

void Memory::SetTimerInterruptEnabled(bool timerInterruptEnabled)
{
	_interruptEnableRegister = SetBit(_interruptEnableRegister, InterruptHandler::TIMER_INTERRUPT_BIT_NUMBER, timerInterruptEnabled);
}

void Memory::SetSerialInterruptEnabled(bool serialInterruptEnabled)
{
	_interruptEnableRegister = SetBit(_interruptEnableRegister, InterruptHandler::SERIAL_INTERRUPT_BIT_NUMBER, serialInterruptEnabled);
}

void Memory::SetJoypadInterruptEnabled(bool joypadInterruptEnabled)
{
	_interruptEnableRegister = SetBit(_interruptEnableRegister, InterruptHandler::JOYPAD_INTERRUPT_BIT_NUMBER, joypadInterruptEnabled);
}

void Memory::SetInterruptEnableRegister(int interruptEnableRegister)
{
	_interruptEnableRegister = interruptEnableRegister;
}

cartridges::Cartridge &Memory::GetCartridge()
{
	return *_cartridge;
}

VideoRam &Memory::GetVideoRam()
{
	return *_videoRam;
}

WorkRam &Memory::GetWorkRam()
{
	return *_workRam;
}

SpriteAttributeRam &Memory::GetSpriteAttributeRam()
{
	return *_spriteAttributeRam;
}

IOPorts &Memory::GetIOPorts()
{
	return *_ioPorts;
}

HighRam &Memory::GetHighRam()
{
	return *_highRam;
}

int &Memory::GetInterruptEnableRegister()
{
	return _interruptEnableRegister;
}

bool Memory::GetVBlankInterruptEnabled()
{
	return GetBit(_interruptEnableRegister, InterruptHandler::VBLANK_INTERRUPT_BIT_NUMBER);
}

bool Memory::GetLCDStatusInterruptEnabled()
{
	return GetBit(_interruptEnableRegister, InterruptHandler::LCD_STATUS_INTERRUPT_BIT_NUMBER);
}

bool Memory::GetTimerInterruptEnabled()
{
	return GetBit(_interruptEnableRegister, InterruptHandler::TIMER_INTERRUPT_BIT_NUMBER);
}

bool Memory::GetSerialInterruptEnabled()
{
	return GetBit(_interruptEnableRegister, InterruptHandler::SERIAL_INTERRUPT_BIT_NUMBER);
}

bool Memory::GetJoypadInterruptEnabled()
{
	return GetBit(_interruptEnableRegister, InterruptHandler::JOYPAD_INTERRUPT_BIT_NUMBER);
}
