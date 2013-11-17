#include "Frontend.hpp"

using namespace james;
using namespace james::core;

Frontend::Frontend()
 : _emulator ()
{
}

Frontend::~Frontend()
{
}

void Frontend::Initialize()
{
	_emulator.Initialize();
	
	_emulator.SetLCD(*this);
	_emulator.SetJoypad(*this);
}

void Frontend::Finalize()
{
	_emulator.Finalize();
}

void Frontend::Reset()
{
	_emulator.Reset();
}

void Frontend::LoadRom(std::string path)
{
	std::vector<int> rom;
	std::ifstream file(path, std::ios::in | std::ios::binary);
	
	if (file.is_open())
	{
		while (!file.eof())
		{
			char byte;
			
			file.get(byte);
			rom.push_back(((int) byte) & 0xFF);
		}
		
		file.close();
	}
	
	_emulator.SetRom(rom);
}

void Frontend::Start()
{
	_emulator.Start();
}

void Frontend::Pause()
{
	_emulator.Pause();
}

bool Frontend::IsPaused()
{
	return _emulator.IsPaused();
}

void Frontend::RenderFrame()
{
	_emulator.RenderFrame();
}

void Frontend::SignalJoypadInterrupt()
{
	_emulator.GetInterruptHandler().SignalJoypadInterrupt();
}

void Frontend::SignalSerialInterrupt()
{
	_emulator.GetInterruptHandler().SignalSerialInterrupt();
}

Emulator &Frontend::GetDevice()
{
	return _emulator;
}
