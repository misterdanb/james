#ifndef _RomOnly_hpp_
#define _RomOnly_hpp_

#include "Cartridge.hpp"

/*
 * To be implemented
 */

namespace gbc
{
	namespace core
	{
		namespace cartridges
		{
			class RomOnly : public Cartridge
			{
			public:
				RomOnly(DynamicArray<int>);
				~RomOnly();
				
				int ReadByte(int);
				void WriteByte(int, int);
			};
		}
	}
}

#endif
