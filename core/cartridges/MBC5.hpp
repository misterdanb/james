#ifndef _MBC5_hpp_
#define _MBC5_hpp_

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
			class MBC5 : public Cartridge
			{
			public:
				MBC5(int[]);
				~MBC5();
				
				int ReadByte(int);
				void WriteByte(int, int);
			};
		}
	}
}

#endif
