#ifndef _Cartridge_hpp_
#define _Cartridge_hpp_

#include <cstring>
#include <algorithm>

#include "../James.hpp"
#include "../MemoryBus.hpp"
#include "Header.hpp"

namespace james
{
	namespace core
	{
		namespace cartridges
		{
			class Cartridge : public IMemoryBus
			{
			public:
				Cartridge(DynamicArray<int> &);
				virtual ~Cartridge();
				
				Header GetHeader();
				
				void SaveRamDumpToFile();
				void LoadRamDumpFromFile();
				
				virtual int ReadByte(int) = GBC_NULL;
				virtual void WriteByte(int, int) = GBC_NULL;
				
				static Cartridge *Create(DynamicArray<int>);
				
			protected:
				// dynamic array, because of size is not known at compile time
				DynamicArray<int> _rom;
				DynamicArray<int> _ram;
				
				Header _header;
				
				int _selectedRomBank;
				int _selectedRamBank;
			};
		}
	}
}

#endif
