#ifndef _Executable_hpp_
#define _Executable_hpp_

namespace james
{
	namespace core
	{
		class Executable
		{
			virtual void ExecuteTicks (int) = 0;
		};
	}
}

#endif
