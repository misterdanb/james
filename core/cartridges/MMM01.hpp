#ifndef _MMM01_hpp_
#define _MMM01_hpp_

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
			class MMM01 : public Cartridge
			{
			public:
				MMM01(int[], int);
				~MMM01();
				
				int ReadByte(int);
				void WriteByte(int, int);
			};
		}
	}
}

#endif
