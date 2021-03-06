#ifndef _RomRam_hpp_
#define _RomRam_hpp_

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
			class RomRam : public Cartridge
			{
			  public:
				RomRam (DynamicArray<int>);
				~RomRam();

				int ReadByte (int);
				void WriteByte (int, int);
			};
		}
	}
}

#endif
