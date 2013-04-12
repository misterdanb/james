#ifndef _Cartridge_hpp_
#define _Cartridge_hpp_

#include <cstring>

#include "../GBC.hpp"
#include "../MemoryBus.hpp"
#include "Header.hpp"

namespace gbc
{
	namespace core
	{
		namespace cartridges
		{
			class Cartridge : public IMemoryBus
			{
			public:
				Cartridge(int[], int);
				virtual ~Cartridge();
				
				Header GetHeader();
				
				virtual int ReadByte(int) = 0;
				virtual void WriteByte(int, int) = 0;
				
				static Cartridge *Create(int[], int);
				
			protected:
				int *_rom;
				int *_ram;
				
				Header _header;
				
				int _selectedRomBank;
				int _selectedRamBank;
			};
		}
	}
}

#endif
