#ifndef _MBC2_hpp_
#define _MBC2_hpp_

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
			class MBC2 : public Cartridge
			{
			public:
				MBC2(int[]);
				~MBC2();
				
				int ReadByte(int);
				void WriteByte(int, int);
			
			private:
				int _ramEnabled;
			};
		}
	}
}

#endif
