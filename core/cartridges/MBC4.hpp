#ifndef _MBC4_hpp_
#define _MBC4_hpp_

#include "Cartridge.hpp"

/*
 * To be implemented
 */

namespace james
{
	namespace core
	{
		namespace cartridges
		{
			class MBC4 : public Cartridge
			{
			public:
				MBC4(DynamicArray<int>);
				~MBC4();
				
				int ReadByte(int);
				void WriteByte(int, int);
			};
		}
	}
}

#endif
