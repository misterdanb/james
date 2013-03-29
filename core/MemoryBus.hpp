#ifndef _MemoryBus_hpp_
#define _MemoryBus_hpp_

namespace gbc
{
	namespace core
	{
		class IMemoryBus
		{
		public:
			virtual ~IMemoryBus();
			virtual int ReadByte(int) = 0;
			virtual void WriteByte(int, int) = 0;
		};
	}
}

#endif
