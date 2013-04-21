#ifndef _MemoryBus_hpp_
#define _MemoryBus_hpp_

#include "GBC.hpp"

namespace gbc
{
	namespace core
	{
		class IMemoryBus
		{
		public:
			virtual ~IMemoryBus();
			
			virtual int ReadByte(int) = GBC_NULL;
			virtual void WriteByte(int, int) = GBC_NULL;
		};
	}
}

#endif
