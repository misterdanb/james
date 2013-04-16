#ifndef _InterruptHandler_hpp_
#define _InterruptHandler_hpp

namespace gbc
{
	namespace core
	{
		class IInterruptHandler
		{
		public:
			virtual ~IInterruptHandler();
			
			virtual void SignalVBlankInterrupt() = 0;
			virtual void SignalLCDStatusInterrupt() = 0;
			virtual void SignalTimerInterrupt() = 0;
			virtual void SignalSerialInterrupt() = 0;
			virtual void SignalJoypadInterrupt() = 0;
		};
	}
}

#endif
