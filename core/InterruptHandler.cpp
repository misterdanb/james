#include "InterruptHandler.hpp"

InterruptHandler::InterruptHandler()
{
}

InterruptHandler::~InterruptHandler()
{
}

void InterruptHandler::SignalVBlankInterrupt()
{
	int newInterruptRequestRegister = SetBit(_bus->ReadByte(INTERRUPT_REQUEST_ADDRESS),
	                                                        VBLANK_INTERRUPT_BIT_NUMBER,
	                                                        true);
	
	_bus->WriteByte(INTERRUPT_REQUEST_ADDRESS, newInterruptRequestRegister);
}

void InterruptHandler::SignalLCDStatusInterrupt()
{
	int newInterruptRequestRegister = SetBit(_bus->ReadByte(INTERRUPT_REQUEST_ADDRESS),
	                                                        LCD_STATUS_INTERRUPT_BIT_NUMBER,
	                                                        true);
	
	_bus->WriteByte(INTERRUPT_REQUEST_ADDRESS, newInterruptRequestRegister);
}

void InterruptHandler::SignalTimerInterrupt()
{
	int newInterruptRequestRegister = SetBit(_bus->ReadByte(INTERRUPT_REQUEST_ADDRESS),
	                                                        TIMER_INTERRUPT_BIT_NUMBER,
	                                                        true);
	
	_bus->WriteByte(INTERRUPT_REQUEST_ADDRESS, newInterruptRequestRegister);
}

void InterruptHandler::SignalSerialInterrupt()
{
	int newInterruptRequestRegister = SetBit(_bus->ReadByte(INTERRUPT_REQUEST_ADDRESS),
	                                                        SERIAL_INTERRUPT_BIT_NUMBER,
	                                                        true);
	
	_bus->WriteByte(INTERRUPT_REQUEST_ADDRESS, newInterruptRequestRegister);
}

void InterruptHandler::SignalJoypadInterrupt()
{
	int newInterruptRequestRegister = SetBit(_bus->ReadByte(INTERRUPT_REQUEST_ADDRESS),
	                                                        JOYPAD_INTERRUPT_BIT_NUMBER,
	                                                        true);
	
	_bus->WriteByte(INTERRUPT_REQUEST_ADDRESS, newInterruptRequestRegister);
}

void InterruptHandler::SetMemoryBus(MemoryBus &bus)
{
	_bus = &bus;
}

MemoryBus &InterruptHandler::GetMemoryBus()
{
	return *_bus;
}
