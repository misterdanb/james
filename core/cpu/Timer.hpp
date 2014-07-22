#ifndef _Timer_hpp_
#define _Timer_hpp_

#include "../RenderContext.hpp"

namespace james
{
	namespace core
	{
		namespace cpu
		{
			class Timer
			{
			  public:

				Timer();
				~Timer();

				void SetRenderContext (RenderContext* rc);
				void Advance (int ticks);

			  private:
				RenderContext* _rc;
				int _ticksPerCycle;
				int _ticksLeftCycle;
				int _timerCounter;
				int _dividerCounter;
			};
		}
	}
}

#endif