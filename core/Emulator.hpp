#ifndef _Emulator_hpp_
#define _Emulator_hpp_

#include "James.hpp"
#include "Memory.hpp"
#include "ClassicRenderer.hpp"
#include "cpu/Processor.hpp"
#include "SerialPort.hpp"
#include "Joypad.hpp"
#include "LCD.hpp"

using namespace james;
using namespace james::core;
using namespace james::core::cpu;
using namespace james::core::cartridges;

namespace james
{
	namespace core
	{
		class Emulator : public NonCopyable
		{
		public:
			// clock speed over 256 will cause timer inaccuracies
			static const int CLOCKING_SPEED = 1;
			
		public:
			Emulator();
			~Emulator();
			
			void Initialize();
			void Finalize();
			
			void Start();
			void Pause();
			
			bool IsPaused();
			
			void Reset();
			
			void RenderScanline();
			void RenderFrame();
			
			void SetRom(DynamicArray<int> &);
			
			void SetLCD(LCD &);
			void SetJoypad(Joypad &);
			void SetSerialPort(SerialPort &);
			void SetCartridge(Cartridge &);
			
			InterruptHandler &GetInterruptHandler();
			
			LCD &GetLCD();
			Joypad &GetJoypad();
			SerialPort &GetSerialPort();
			Cartridge &GetCartridge();

            Processor &GetProcessor();
            Memory &GetMemory();

            Renderer &GetRenderer();
		
		private:
			void ExecuteMachineClocks(int);
			void UpdateTimer(int);
			
		private:
			Processor _hybr1s80;
			Memory _memory;
			
			// memory parts
			VideoRam *_videoRam;
			WorkRam *_workRam;
			SpriteAttributeRam *_spriteAttributeRam;
			IOPorts *_ioPorts;
			HighRam *_highRam;
			
			// interrupt handler
			InterruptHandler *_interruptHandler;
			
			// emulation stats
			bool _paused;
			int _pendingClocks;
			int _speedFactor;
			
			// physical interfaces
			LCD *_lcd;
			Joypad *_joypad;
			SerialPort *_serialPort;
			Cartridge *_cartridge;
			
			bool _forceClassicGameboy;
			
			// timer
			int _timerTicks;
			int _dividerTicks;
			
			Renderer *_renderer;
		};
	}
}

#endif
