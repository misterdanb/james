#ifndef _MemoryBlock_hpp
#define _MemoryBlock_hpp

#include "James.hpp"
#include "Serializable.hpp"

namespace james
{
	namespace core
	{
		/*
		 * MemoryBlock is an abstract implementation for all classes
		 * that use banked or non-banked memory.
		 */
		
		class MemoryBlock : public Serializable
		{
		public:
			MemoryBlock(int, int);
			~MemoryBlock();
			
			int ReadByteFromBank(int, int);
			void WriteByteToBank(int, int, int);
			
			virtual void Serialize(std::ostream &);
			virtual void Deserialize(std::istream &);
			
			virtual void Reset();
			
		private:
			int _banks;
			int _size;
			
			DynamicArray2<int> _data;
		};
	}
}

#endif
