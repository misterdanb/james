#ifndef _SpriteAttributeRam_hpp_
#define _SpriteAttributeRam_hpp_

#include "James.hpp"

#include "MemoryBlock.hpp"
#include "MemoryBus.hpp"
#include "SpriteAttribute.hpp"

namespace james
{
	namespace core
	{
		class SpriteAttributeRam : public MemoryBlock, public MemoryBus
		{
		public:
			static const int SPRITE_ATTRIBUTE_RAM_OFFSET = 0xFE00;
			
			static const int SPRITE_ATTRIBUTE_RAM_BANKS = 1;
			static const int SPRITE_ATTRIBUTE_RAM_SIZE = 0x0100;
			
		public:
			SpriteAttributeRam();
			~SpriteAttributeRam();
			
			int ReadByte(int);
			void WriteByte(int, int);
			
			void SetSpriteAttribute(int, SpriteAttribute);
			
			SpriteAttribute GetSpriteAttribute(int);
		};
	}
}

#endif
