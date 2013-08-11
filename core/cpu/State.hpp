#ifndef _State_hpp_
#define _State_hpp_

namespace james
{
	namespace core
	{
		namespace cpu
		{
			class State
			{
			public:
				int a, f;
				int b, c;
				int d, e;
				int h, l;
				int pc;
				int sp;
	
				int ticks;
	
				int interruptsEnabled;
				int stopped;
				int halted;
			};
		}
	}
}

#endif
