#ifndef _InterruptHandler_hpp_
#define _InterruptHandler_hpp_

#include "James.hpp"

#include "MemoryBus.hpp"

using namespace james;
using namespace james::core;

namespace james
{
	namespace core
	{
		class InterruptHandler
		{
		public:
			// interrupt registers and bits
			static const int INTERRUPT_ENABLE_ADDRESS = 0xFFFF;
			static const int INTERRUPT_REQUEST_ADDRESS = 0xFF0F;
			
			static const int VBLANK_INTERRUPT_BIT_NUMBER = 0;
			static const int LCD_STATUS_INTERRUPT_BIT_NUMBER = 1;
			static const int TIMER_INTERRUPT_BIT_NUMBER = 2;
			static const int SERIAL_INTERRUPT_BIT_NUMBER = 3;
			static const int JOYPAD_INTERRUPT_BIT_NUMBER = 4;
			
			static const int VBLANK_INTERRUPT_VECTOR = 0x40;
			static const int LCD_STATUS_INTERRUPT_VECTOR = 0x48;
			static const int TIMER_INTERRUPT_VECTOR = 0x50;
			static const int SERIAL_INTERRUPT_VECTOR = 0x58;
			static const int JOYPAD_INTERRUPT_VECTOR = 0x60;
		
		public:
			InterruptHandler();
			~InterruptHandler();
			
			void SignalVBlankInterrupt();
			void SignalLCDStatusInterrupt();
			void SignalTimerInterrupt();
			void SignalSerialInterrupt();
			void SignalJoypadInterrupt();
			
			void SetMemoryBus(MemoryBus &);
			
			MemoryBus &GetMemoryBus();
		
		private:
			MemoryBus *_bus;
		};
	}
}

#endif
