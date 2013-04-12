#ifndef _RomRam_hpp_
#define _RomRam_hpp_

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
			class RomRam : public Cartridge
			{
			public:
				RomRam(int[], int);
				~RomRam();
				
				int ReadByte(int);
				void WriteByte(int, int);
			};
		}
	}
}

#endif
