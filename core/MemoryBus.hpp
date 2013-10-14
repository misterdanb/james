#ifndef _MemoryBus_hpp_
#define _MemoryBus_hpp_

#include "James.hpp"

using namespace james;
using namespace james::core;

namespace james
{
	namespace core
	{
		/*
		 * MemoryBus is an interface to all classes that represent an
		 * addressable part of the memory.
		 */
		
		class MemoryBus
		{
		public:
			virtual ~MemoryBus();
			
			/*int &operator[](int)
			{
			}*/
			
			virtual int ReadByte(int) = GBC_NULL;
			virtual void WriteByte(int, int) = GBC_NULL;
		};
	}
}

#endif
