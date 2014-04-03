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
        class Memory : public NonCopyable, public MemoryBus, public Serializable
        {
        public:
            /**
             * Constructor of Memory.
             * 
             * Initializes with the default values.
             */
            Memory();
            
            /**
             * Destructor of Memory.
             */
            ~Memory() = default;
            
            int ReadByte(int);
            void WriteByte(int, int);
            
            void Serialize(std::ostream &);
            void Deserialize(std::istream &);
            
            /**
             * Modifies the cartridge.
             * 
             * @param cartrige New value of the cartridge.
             */
            void SetCartridge(Cartridge &cartridge);
            
            /**
             * Modifies the video ram.
             * 
             * @param videoRam New  video ram.
             */
            void SetVideoRam(VideoRam &videoRam);
            
            /**
             * Modifies the work ram.
             * 
             * @param workRam New work ram.
             */
            void SetWorkRam(WorkRam &workRam);
            
            /**
             * Modifies the sprite attribute ram.
             * 
             * @param spriteAttributeRam New sprite attribute ram.
             */
            void SetSpriteAttributeRam(SpriteAttributeRam &spriteAttributeRam);
            
            /**
             * Modifies the io ports.
             * 
             * @param ioPorts New io ports.
             */
            void SetIOPorts(IOPorts &ioPorts);
            
            /**
             * Modifies the high ram
             * 
             * @param highRam New the high ram.
             */
            void SetHighRam(HighRam &highRam);
            
            /**
             * Modifies the interrupt enable register.
             * 
             * @param interruptEnableRegister New interrupt enable
             *        register.
             */
            void SetInterruptEnableRegister(int interruptEnableRegister);
            
            /**
             * Modifies the vblank interrupt enabled bit of the
             * interrupt enable register.
             * 
             * @param vBlankInterruptEnabled New vblank interrupt
             *        enabled bit.
             */
            void SetVBlankInterruptEnabled(bool vBlankInterruptEnabled);
            
            /**
             * Modifies the lcd status interrupt enabled bit of the
             * interrupt enable register.
             * 
             * @param lcdStatusInterruptEnabled New lcd status interrupt
             *        enabled bit.
             */
            void SetLCDStatusInterruptEnabled(bool lcdStatusInterruptEnabled);
            
            /**
             * Modifies the timer interrupt enabled bit of the interrupt
             * enable register.
             * 
             * @param timerInterruptEnabled New timer interrupt enabled
             *        bit.
             */
            void SetTimerInterruptEnabled(bool timerInterruptEnabled);
            
            /**
             * Modifies the serial interrupt enabled bit of the
             * interrupt enable register.
             * 
             * @param serialInterruptEnabled New value of the serial
             *        interrupt enabled bit.
             */
            void SetSerialInterruptEnabled(bool serialInterruptEnabled);
            
            /**
             * Modifies the joypad interrupt enabled bit of the
             * interrupt enable register.
             * 
             * @param joypadInterruptEnabled New joypad interrupt
             *        enabled bit.
             */
            void SetJoypadInterruptEnabled(bool);
            
            /**
             * Returns the cartridge.
             * 
             * @return The cartridge.
             */
            Cartridge &GetCartridge();
            
            /**
             * Returns the video ram.
             * 
             * @return The video ram.
             */
            VideoRam &GetVideoRam();
            
            /**
             * Returns the work ram.
             * 
             * @return The work ram.
             */
            WorkRam &GetWorkRam();
            
            /**
             * Returns the sprite attribute table.
             * 
             * @return The sprite attribute table.
             */
            SpriteAttributeRam &GetSpriteAttributeRam();
            
            /**
             * Returns the io ports.
             * 
             * @return The io ports.
             */
            IOPorts &GetIOPorts();
            
            /**
             * Returns the high ram.
             * 
             * @return The high ram.
             */
            HighRam &GetHighRam();
            
            /**
             * Returns the interrupt enable register.
             * 
             * @return The interrupt enable register.
             */
            int GetInterruptEnableRegister();
            
            /**
             * Returns the vblank interrupt enabled bit of the interrupt
             * enable register.
             * 
             * @return The vblank interrupt enabled bit.
             */
            bool GetVBlankInterruptEnabled();
            
            /**
             * Returns the lcd status interrupt enabled bit of the
             * interrupt enable register.
             * 
             * @return The lcd status interrupt enabled bit.
             */
            bool GetLCDStatusInterruptEnabled();
            
            /**
             * Returns the timer interrupt enabled bit of the interrupt
             * enable register.
             * 
             * @return The timer interrupt enabled bit.
             */
            bool GetTimerInterruptEnabled();
            
            /**
             * Returns the serial interrupt enabled bit of the interrupt
             * enable register.
             * 
             * @return The serial interrupt enabled bit.
             */
            bool GetSerialInterruptEnabled();
            
            /**
             * Returns the joybad interrupt enabled bit of the interrupt
             * enable register.
             * 
             * @return The joybad interrupt enabled bit.
             */
            bool GetJoypadInterruptEnabled();
        
        private:
            Cartridge *_cartridge;
            VideoRam *_videoRam;
            WorkRam *_workRam;
            SpriteAttributeRam *_spriteAttributeRam;
            IOPorts *_ioPorts;
            HighRam *_highRam;
            
            /**
             * The Register in which the physically different interrupts
             * are enabled or disabled. The format of this register
             * looks like this where a 1 means the corresponding
             * interrupt is enabled and 0 means it is disabled:
             * 
             * <code>
             * Bit 0: vBlankInterruptEnabled (interrupt vector is 0x40)
             * Bit 1: lcdStatusInterruptEnabled (interrupt vector is 0x48)
             * Bit 2: timerInterruptEnabled (interrupt vector is 0x50)
             * Bit 3: serialInterruptEnabled (interrupt vector is 0x58)
             * Bit 4: joypadInterruptEnabled (interrupt vector is 0x60)
             * Bit 5: not used
             * Bit 6: not used
             * Bit 7: not used
             * </code>
             */
            int _interruptEnableRegister;
        };
    }
}

#endif
