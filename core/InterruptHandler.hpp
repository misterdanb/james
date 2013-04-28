#ifndef _InterruptHandler_hpp_
#define _InterruptHandler_hpp_

#include "gbc.hpp"

namespace gbc
{
	namespace core
	{
		class IInterruptHandler
		{
		public:
			// interrupt registers and bits
			static const int INTERRUPT_ENABLE_ADDRESS = 0xFFFF;
			static const int INTERRUPT_REQUEST_ADDRESS = 0xFF0F;
			
			static const int VERTICAL_BLANK_INTERRUPT_ENABLE_BIT = 0;
			static const int LCD_STATUS_INTERRUPT_ENABLE_BIT = 1;
			static const int TIMER_INTERRUPT_ENABLE_BIT = 2;
			static const int SERIAL_INTERRUPT_ENABLE_BIT = 3;
			static const int JOYPAD_INTERRUPT_ENABLE_BIT = 4;
			
			static const int VERTICAL_BLANK_INTERRUPT_REQUEST_BIT = 0;
			static const int LCD_STATUS_INTERRUPT_REQUEST_BIT = 1;
			static const int TIMER_INTERRUPT_REQUEST_BIT = 2;
			static const int SERIAL_INTERRUPT_REQUEST_BIT = 3;
			static const int JOYPAD_INTERRUPT_REQUEST_BIT = 4;
			
			static const int VERTICAL_BLANK_INTERRUPT_VECTOR = 0x40;
			static const int LCD_STATUS_INTERRUPT_VECTOR = 0x48;
			static const int TIMER_INTERRUPT_VECTOR = 0x50;
			static const int SERIAL_INTERRUPT_VECTOR = 0x58;
			static const int JOYPAD_INTERRUPT_VECTOR = 0x60;
		
		public:
			virtual ~IInterruptHandler();
			
			virtual void SignalVBlankInterrupt() = GBC_NULL;
			virtual void SignalLCDStatusInterrupt() = GBC_NULL;
			virtual void SignalTimerInterrupt() = GBC_NULL;
			virtual void SignalSerialInterrupt() = GBC_NULL;
			virtual void SignalJoypadInterrupt() = GBC_NULL;
		};
	}
}

#endif
