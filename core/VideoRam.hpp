#ifndef _VideoRam_hpp_
#define _VideoRam_hpp

#include "James.hpp"

#include "MemoryBlock.hpp"
#include "MemoryBus.hpp"
#include "IOPorts.hpp"
#include "Tile.hpp"
#include "BackgroundAttribute.hpp"
#include "BackgroundTileNumberMap.hpp"
#include "BackgroundAttributeMap.hpp"

namespace james
{
	namespace core
	{
		class VideoRam : public MemoryBlock, public MemoryBus
		{
		public:
			static const int VIDEO_RAM_OFFSET = 0x8000;
			
			static const int VIDEO_RAM_BANKS = 2;
			static const int VIDEO_RAM_BANK_SIZE = 0x2000;
			
		public:
			VideoRam();
			~VideoRam();
			
			int ReadByte(int);
			void WriteByte(int, int);
			
			void SetIOPorts(IOPorts &);
			
			void SetTile(int, Tile);
			
			void SetBackgroundTileNumber(int, int, int);
			void SetBackgroundTileNumberMap(int, BackgroundTileNumberMap);
			void SetBackgroundAttribute(int, int, BackgroundAttribute);
			void SetBackgroundAttributeMap(int, BackgroundAttributeMap);
			
			IOPorts &GetIOPorts();
			
			Tile GetTile(int);
			
			int GetBackgroundTileNumber(int, int);
			BackgroundTileNumberMap GetBackgroundTileNumberMap(int);
			BackgroundAttribute GetBackgroundAttribute(int);
			BackgroundAttributeMap GetBackgroundAttributeMap(int);
		
		private:
			IOPorts &_ioPorts;
		};
	}
}

#endif
