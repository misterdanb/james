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
        /**
         * @class Memory
         * 
         * Memory manages all parts connected to the game boy bus
         * and is the central unit of it. All read and write operations
         * on the bus must be done on this class, if they shell behave
         * like on an actual game boy.
         */
        class Memory : public NonCopyable, MemoryBus, public Serializable
        {
        public:
            /**
             * Constructor of Memory.
             * 
             * Initializes all members with their default values.
             */
            Memory();
            
            /**
             * Decstructor of Memory.
             */
            ~Memory();
            
            int ReadByte(int);
            void WriteByte(int, int);
            
            void Serialize(std::ostream &);
            void Deserialize(std::istream &);
            
            /**
             * Sets the cartridge.
             * 
             * @param cartrige Reference of the cartridge.
             */
            void SetCartridge(Cartridge &cartridge);
            
            /**
             * Sets the video ram.
             * 
             * @param videoRam Reference of the video ram.
             */
            void SetVideoRam(VideoRam &videoRam);
            
            /**
             * Sets the work ram.
             * 
             * @param workRam Reference of the work ram.
             */
            void SetWorkRam(WorkRam &workRam);
            
            /**
             * Sets the sprite attribute ram.
             * 
             * @param spriteAttributeRam Reference of the sprite
             *                           attribute ram.
             */
            void SetSpriteAttributeRam(SpriteAttributeRam &spriteAttributeRam);
            
            /**
             * Sets the io ports.
             * 
             * @param ioPorts Reference of the io ports.
             */
            void SetIOPorts(IOPorts &ioPorts);
            
            /**
             * Sets the high ram
             * 
             * @param highRam Reference of the high ram.
             */
            void SetHighRam(HighRam &highRam);
            
            /**
             * Sets the interrupt enable register.
             * 
             * @param interruptEnableRegister New value of the interrupt
             *                                enable register.
             */
            void SetInterruptEnableRegister(int interruptEnableRegister);
            
            /**
             * Sets the vblank interrupt enabled bit of the interrupt
             * enable register.
             * 
             * @param vBlankInterruptEnabled New value of the vblank
             *                               interrupt enabled bit.
             */
            void SetVBlankInterruptEnabled(bool vBlankInterruptEnabled);
            
            /**
             * Sets the lcd status interrupt enabled bit of the
             * interrupt enable register.
             * 
             * @param lcdStatusInterruptEnabled New value of the lcd
             *                                  status interrupt enabled
             *                                  bit.
             */
            void SetLCDStatusInterruptEnabled(bool lcdStatusInterruptEnabled);
            
            /**
             * Sets the timer interrupt enabled bit of the interrupt
             * enable register.
             * 
             * @param timerInterruptEnabled New value of the timer
             *                              interrupt enabled bit.
             */
            void SetTimerInterruptEnabled(bool timerInterruptEnabled);
            
            /**
             * Sets the serial interrupt enabled bit of the interrupt
             * enable register.
             * 
             * @param serialInterruptEnabled New value of the serial
             *                               interrupt enabled bit.
             */
            void SetSerialInterruptEnabled(bool serialInterruptEnabled);
            
            /**
             * Sets the joypad interrupt enabled bit of the interrupt
             * enable register.
             * 
             * @param joypadInterruptEnabled New value of the joypad
             *                               interrupt enabled bit.
             */
            void SetJoypadInterruptEnabled(bool);
            
            /**
             * Gets the cartridge.
             * 
             * @return Reference of the cartridge.
             */
            Cartridge &GetCartridge();
            
            /**
             * Gets the video ram.
             * 
             * @return Reference of the video ram.
             */
            VideoRam &GetVideoRam();
            
            /**
             * Gets the work ram.
             * 
             * @return Reference of the work ram.
             */
            WorkRam &GetWorkRam();
            
            /**
             * Gets the sprite attribute table.
             * 
             * @return Reference of the sprite attribute table.
             */
            SpriteAttributeRam &GetSpriteAttributeRam();
            
            /**
             * Gets the io ports.
             * 
             * @return Reference of the io ports.
             */
            IOPorts &GetIOPorts();
            
            /**
             * Gets the high ram.
             * 
             * @return Reference of the high ram.
             */
            HighRam &GetHighRam();
            
            /**
             * Gets the interrupt enable register.
             * 
             * @return Value of the interrupt enable register.
             */
            int GetInterruptEnableRegister();
            
            /**
             * Gets the vblank interrupt enabled bit of the interrupt
             * enable register.
             * 
             * @return Value of the vblank interrupt enabled bit.
             */
            bool GetVBlankInterruptEnabled();
            
            /**
             * Gets the lcd status interrupt enabled bit of the
             * interrupt enable register.
             * 
             * @return Value of the lcd status interrupt enabled bit.
             */
            bool GetLCDStatusInterruptEnabled();
            
            /**
             * Gets the timer interrupt enabled bit of the interrupt
             * enable register.
             * 
             * @return Value of the timer interrupt enabled bit.
             */
            bool GetTimerInterruptEnabled();
            
            /**
             * Gets the serial interrupt enabled bit of the interrupt
             * enable register.
             * 
             * @return Value of the serial interrupt enabled bit.
             */
            bool GetSerialInterruptEnabled();
            
            /**
             * Gets the joybad interrupt enabled bit of the interrupt
             * enable register.
             * 
             * @return Value of the joybad interrupt enabled bit.
             */
            bool GetJoypadInterruptEnabled();
        
        private:
            /**
             * The cartridge bound into the system bus.
             */
            Cartridge *_cartridge;
            
            /**
             * The video ram bound into the system bus.
             */
            VideoRam *_videoRam;
            
            /**
             * The work ram bound into the system bus.
             */
            WorkRam *_workRam;
            
            /**
             * The sprite attribute table bound into the system bus.
             */
            SpriteAttributeRam *_spriteAttributeRam;
            
            /**
             * The io ports bound into the system bus.
             */
            IOPorts *_ioPorts;
            
            /**
             * The high ram bound into the system bus.
             */
            HighRam *_highRam;
            
            /**
             * Register in which the physically different interrupts
             * are enabled or disabled. The format of this register
             * looks like this where a 1 means the corresponding
             * interrupt is enabled and 0 means it is disabled:
             * 
             * Bit 0: VBlankInterruptEnabled (interrupt vector is 0x40)
             * Bit 1: LCDStatusInterruptEnabled (interrupt vector is 0x48)
             * Bit 2: TimerInterruptEnabled (interrupt vector is 0x50)
             * Bit 3: SerialInterruptEnabled (interrupt vector is 0x58)
             * Bit 4: JoypadInterruptEnabled (interrupt vector is 0x60)
             */
            int _interruptEnableRegister;
        };
    }
}

#endif
