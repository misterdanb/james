#ifndef _MBC3_hpp_
#define _MBC3_hpp_

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
			class MBC3 : public Cartridge
			{
			public:
				MBC3(int[]);
				~MBC3();
				
				int ReadByte(int);
				void WriteByte(int, int);
			};
		}
	}
}

#endif
