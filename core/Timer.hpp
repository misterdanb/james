#ifndef _Timer_hpp_
#define _Timer_hpp_

#include "James.hpp"
#include "RenderContext.hpp"
#include "Executable.hpp"

namespace james
{
	namespace core
	{
		class Timer : public Executable
		{
		public:
			static const int DEVIDER_REGISTER_ADDRESS = 0xFF04;
			static const int TIMER_COUNTER_ADDRESS = 0xFF05;
			static const int TIMER_MODULO_ADDRESS = 0xFF06;
			static const int TIMER_CONTROL_ADDRESS = 0xFF07;

		public:
			Timer();
			~Timer();

			void SetRenderContext (RenderContext& rc);
			void ExecuteTicks (int);

		private:
			RenderContext* _rc;

			int _ticksPerCycle;
			int _ticksLeftCycle;
			int _timerCounter;
			int _dividerCounter;
		};
	}
}

#endif
