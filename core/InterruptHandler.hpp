#ifndef _InterruptHandler_hpp_
#define _InterruptHandler_hpp

#include "GBC.hpp"

namespace gbc
{
	namespace core
	{
		class IInterruptHandler
		{
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
