#include "Frontend.hpp"

using namespace james;
using namespace james::core;

Frontend::Frontend()
{
}

Frontend::~Frontend()
{
}

void Frontend::Initialize()
{
	_gbc.Initialize();

	_gbc.SetLCD (*this);
	_gbc.SetJoypad (*this);
}

void Frontend::Finalize()
{
	_gbc.Finalize();
}

void Frontend::Reset()
{
	_gbc.Reset();
}

void Frontend::LoadRom (std::string path)
{
	std::vector<int> rom;
	std::ifstream file (path, std::ios::in | std::ios::binary);

	if (file.is_open())
	{
		while (!file.eof())
		{
			char byte;

			file.get (byte);
			rom.push_back (((int) byte) & 0xFF);
		}

		file.close();
	}

	_gbc.SetRom (rom);
}

void Frontend::Start()
{
	_gbc.Start();
}

void Frontend::Pause()
{
	_gbc.Pause();
}

bool Frontend::IsPaused()
{
	return _gbc.IsPaused();
}

void Frontend::RenderFrame()
{
	_gbc.RenderFrame();
}

void Frontend::SignalJoypadInterrupt()
{
	_gbc.GetInterruptHandler().SignalJoypadInterrupt();
}

void Frontend::SignalSerialInterrupt()
{
	_gbc.GetInterruptHandler().SignalSerialInterrupt();
}

GameboyColor& Frontend::GetDevice()
{
	return _gbc;
}
