#ifndef _HighRam_hpp_
#define _HighRam_hpp_

#include "MemoryBlock.hpp"
#include "MemoryBus.hpp"

namespace james
{
	namespace core
	{
		class HighRam : public MemoryBlock, public MemoryBus
		{
		public:
			static const int HIGH_RAM_OFFSET = 0xFF80;
			
			static const int HIGH_RAM_BANKS = 1;
			static const int HIGH_RAM_BANK_SIZE = 0x007F;
			
		public:
			HighRam();
			~HighRam();
			
			int ReadByte(int);
			void WriteByte(int, int);
		};
	}
}

#endif
