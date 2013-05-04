#ifndef _MBC1_hpp_
#define _MBC1_hpp_

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
			class MBC1 : public Cartridge
			{
			public:
				MBC1(DynamicArray<int>);
				~MBC1();
				
				int ReadByte(int);
				void WriteByte(int, int);
			
			private:
				int _ramEnabled;
				int _romRamMode;
			};
		}
	}
}

#endif
