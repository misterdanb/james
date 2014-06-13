#ifndef _MBC5_hpp_
#define _MBC5_hpp_

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
			class MBC5 : public Cartridge
			{
			  public:
				MBC5 (DynamicArray<int>);
				~MBC5();

				int ReadByte (int);
				void WriteByte (int, int);

			  private:
				int _ramEnabled;
				int _romRamMode;
			};
		}
	}
}

#endif
