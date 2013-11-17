#include "Emulator.hpp"

using namespace james;
using namespace james::core;

Emulator::Emulator()
	: _hybr1s80(),
	  _memory(),
	  _videoRam(nullptr),
	  _workRam(nullptr),
	  _spriteAttributeRam(nullptr),
	  _ioPorts(nullptr),
	  _highRam(nullptr),
	  _interruptHandler(),
	  _paused(true),
	  _pendingClocks(0),
	  _speedFactor(1),
	  _lcd(NULL),
	  _joypad(NULL),
	  _serialPort(NULL),
	  _cartridge(NULL),
	  _forceClassicGameboy(true), // for testing
	  _timerTicks(0),
	  _dividerTicks(0),
	  _renderer(nullptr)
{
}

Emulator::~Emulator()
{
}

void Emulator::Initialize()
{
	LOG("Initializing.");
	
	_videoRam.reset(new VideoRam());
	_workRam.reset(new WorkRam());
	_spriteAttributeRam.reset(new SpriteAttributeRam());
	_ioPorts.reset(new IOPorts());
	_highRam.reset(new HighRam());
	
	_interruptHandler.reset(new InterruptHandler());
	
	_memory.SetVideoRam(*_videoRam);
	_memory.SetWorkRam(*_workRam);
	_memory.SetSpriteAttributeRam(*_spriteAttributeRam);
	_memory.SetIOPorts(*_ioPorts);
	_memory.SetHighRam(*_highRam);

	if (_cartridge->GetHeader().platformSupport == PlatformSupport::GAMEBOY_COLOR_SUPPORT ||
	    _cartridge->GetHeader().platformSupport != PlatformSupport::GAMEBOY_COLOR_ONLY)
	{
		_renderer.reset(new ClassicRenderer());
		
		(*_renderer).SetMemory(_memory);
		(*_renderer).SetInterruptHandler(*_interruptHandler);
		
		LOG("Using Gameboy Classic rendering method.");
	}

	_hybr1s80.SetMemoryBus(_memory);
	(*_ioPorts).SetMemoryBus(_memory);
	(*_interruptHandler).SetMemoryBus(_memory);
	
	(*_videoRam).SetIOPorts(*_ioPorts);
	(*_workRam).SetIOPorts(*_ioPorts);
	
	Reset();
}

void Emulator::Finalize()
{
	LOG("Finalizing.");
}

void Emulator::Start()
{
	_paused = false;
	
	LOG("Started emulation.");
}

void Emulator::Pause()
{
	_paused = true;
	
	LOG("Paused emulation.");
}

bool Emulator::IsPaused()
{
	return _paused;
}

void Emulator::Reset()
{
	(*_videoRam).Reset();
	(*_workRam).Reset();
	(*_spriteAttributeRam).Reset();
	(*_ioPorts).Reset();
	(*_highRam).Reset();
	
	_hybr1s80.PowerUp();
}

void Emulator::RenderScanline()
{
	if ((*_ioPorts).GetLCDY() < 144)
	{
		(*_ioPorts).SetLCDMode(LCDMode::SEARCHING_OAM);
		
		ExecuteMachineClocks(80 * _speedFactor);
		(*_renderer).RenderOAMSearch();
		
		(*_ioPorts).SetLCDMode(LCDMode::TRANSFERRING_DATA);
		
		ExecuteMachineClocks(172 * _speedFactor);
		(*_renderer).RenderTransferData();
		
		(*_ioPorts).SetLCDMode(LCDMode::HORIZONTAL_BLANK);
		
		ExecuteMachineClocks(204 * _speedFactor);
		(*_renderer).RenderHBlank();
	}
	else
	{
		(*_ioPorts).SetLCDMode(LCDMode::VERTICAL_BLANK);
		
		ExecuteMachineClocks(456 * _speedFactor);
		(*_renderer).RenderVBlank();
	}
}

void Emulator::RenderFrame()
{
	if (!_paused)
	{
		for (int i = 0; i < 154; i++)
		{
			RenderScanline();
		}
	}
	
	if (_lcd)
	{
		(*_lcd).DrawFrame((*_renderer).GetFrame());
	}
	else
	{
		ERROR("Unable to draw a frame: No LCD set.");
	}
}

void Emulator::SetRom(DynamicArray<int> &rom)
{
	_cartridge = Cartridge::Create(rom);

	std::string path = ToHex(_cartridge->GetHeader().globalChecksum[0]) +
	                   ToHex(_cartridge->GetHeader().globalChecksum[1]) +
	                   std::string(".battery\0");

	std::ifstream file(path, std::ios::binary);

	if (file.is_open() && !file.bad())
	{
		file >> (*_cartridge);
		file.close();

		LOG("Loaded cartridge ram battery.");
	}
	else
	{
		ERROR("Failed to load cartridge ram battery.");
	}

	LOG("Loaded cartridge");

//	if (_cartridge->GetHeader().platformSupport == PlatformSupport::GAMEBOY_COLOR_SUPPORT ||
//	    _cartridge->GetHeader().platformSupport != PlatformSupport::GAMEBOY_COLOR_ONLY)
//	{
//		_renderer.reset(new ClassicRenderer());
//		
//		(*_renderer).SetMemory(_memory);
//
//		LOG("Using Gameboy Classic rendering method.");
//	}
	_memory.SetCartridge(_cartridge);
}

void Emulator::SetLCD(LCD &lcd)
{
	_lcd = &lcd;
}

void Emulator::SetJoypad(Joypad &joypad)
{
	_joypad = &joypad;
	
	(*_ioPorts).SetJoypad(joypad);
}

void Emulator::SetSerialPort(SerialPort &serialPort)
{
	_serialPort = &serialPort;
	
	(*_ioPorts).SetSerialPort(serialPort);
}

InterruptHandler &Emulator::GetInterruptHandler()
{
	return *_interruptHandler;
}

LCD &Emulator::GetLCD()
{
	return (*_lcd);
}

Joypad &Emulator::GetJoypad()
{
	return (*_joypad);
}

SerialPort &Emulator::GetSerialPort()
{
	return (*_serialPort);
}

Cartridge &Emulator::GetCartridge()
{
	return (*_cartridge);
}

void Emulator::ExecuteMachineClocks(int clocks)
{
	_pendingClocks += clocks;
	
	while (_pendingClocks >= CLOCKING_SPEED)
	{
		_pendingClocks -= CLOCKING_SPEED;
		
		_hybr1s80.Execute(CLOCKING_SPEED * 4);
		
		UpdateTimer(CLOCKING_SPEED * 4);
	}
}

void Emulator::UpdateTimer(int ticks)
{
	if (!(*_ioPorts).GetTimerStopped())
	{
		_timerTicks += ticks;
		//(*_ioPorts).SetTimerCounter((*ioPorts).GetTimerCounter() + ticks);
		
		if (_timerTicks >= (*_ioPorts).GetTimerClockFrequency())
		{
			int newTimerCounter = (*_ioPorts).GetTimerCounter() + _timerTicks / (*_ioPorts).GetTimerClockFrequency();
			
			(*_ioPorts).SetTimerCounter(newTimerCounter);
			
			if (newTimerCounter > 0xFF)
			{
				(*_ioPorts).SetTimerCounter((*_ioPorts).GetTimerModulo());
				
				(*_interruptHandler).SignalTimerInterrupt();
			}
			
			_timerTicks %= (*_ioPorts).GetTimerClockFrequency();
		}
	}
	
	_dividerTicks += ticks;
		
	if ((*_ioPorts).GetDivider() >= 0x100)
	{
		(*_ioPorts).SetDivider((*_ioPorts).GetDivider() + _dividerTicks / 0x100);
		
		_dividerTicks %= 0x100;
	}
}
