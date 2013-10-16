#ifndef _WorkRam_hpp_
#define _WorkRam_hpp_

#include "James.hpp"

#include "MemoryBlock.hpp"
#include "MemoryBus.hpp"
#include "IOPorts.hpp"

namespace james
{
	namespace core
	{
		class WorkRam : public MemoryBlock, public MemoryBus
		{
		public:
			static const int WORK_RAM_OFFSET_1 = 0xC000;
			static const int WORK_RAM_OFFSET_2 = 0xD000;
			static const int WORK_RAM_OFFSET_3 = 0xE000;
			static const int WORK_RAM_OFFSET_4 = 0xF000;
			
			static const int WORK_RAM_BANKS = 8;
			static const int WORK_RAM_BANK_SIZE = 0x1000;
			
		public:
			WorkRam();
			~WorkRam();
			
			int ReadByte(int);
			void WriteByte(int, int);
			
			void SetIOPorts(IOPorts &);
			
			IOPorts &GetIOPorts();
			
		private:
			IOPorts *_ioPorts;
		};
	}
}

#endif
