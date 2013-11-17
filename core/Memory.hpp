#ifndef _Memory_hpp_
#define _Memory_hpp_

#include <memory>

#include "James.hpp"

#include "cartridges/Cartridge.hpp"
#include "VideoRam.hpp"
#include "WorkRam.hpp"
#include "SpriteAttributeRam.hpp"
#include "IOPorts.hpp"
#include "HighRam.hpp"
#include "InterruptHandler.hpp"

using namespace james;
using namespace james::core;
using namespace james::core::cartridges;

namespace james
{
	namespace core
	{
		/*
		 * Memory manages all parts connected to the game boy bus
		 * and is the central unit of it. All reads from and writes
		 * to the bus belong to this class.
		 */
		
		class Memory : public MemoryBus, public Serializable
		{
		public:
			Memory();
			~Memory();
			
			int ReadByte(int);
			void WriteByte(int, int);
			
			void Serialize(std::ostream &);
			void Deserialize(std::istream &);
			
			void SetCartridge(cartridges::Cartridge *);
			void SetVideoRam(VideoRam &);
			void SetWorkRam(WorkRam &);
			void SetSpriteAttributeRam(SpriteAttributeRam &);
			void SetIOPorts(IOPorts &);
			void SetHighRam(HighRam &);
			void SetInterruptEnableRegister(int);
			
			void SetVBlankInterruptEnabled(bool);
			void SetLCDStatusInterruptEnabled(bool);
			void SetTimerInterruptEnabled(bool);
			void SetSerialInterruptEnabled(bool);
			void SetJoypadInterruptEnabled(bool);
			
			cartridges::Cartridge &GetCartridge();
			VideoRam &GetVideoRam();
			WorkRam &GetWorkRam();
			SpriteAttributeRam &GetSpriteAttributeRam();
			IOPorts &GetIOPorts();
			HighRam &GetHighRam();
			int &GetInterruptEnableRegister();
			
			bool GetVBlankInterruptEnabled();
			bool GetLCDStatusInterruptEnabled();
			bool GetTimerInterruptEnabled();
			bool GetSerialInterruptEnabled();
			bool GetJoypadInterruptEnabled();
		
		private:
			Cartridge *_cartridge;
			VideoRam *_videoRam;
			WorkRam *_workRam;
			SpriteAttributeRam *_spriteAttributeRam;
			IOPorts *_ioPorts;
			HighRam *_highRam;
			int _interruptEnableRegister;
			Memory(const james::core::Memory&); 
			Memory &operator=(const james::core::Memory&);
		};
	}
}

#endif
