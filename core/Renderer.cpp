#include "Renderer.hpp"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}


void Renderer::SetMemory(Memory &memory)
{
	_memory = &memory;
	
	_videoRam = &memory.GetVideoRam();
	_spriteAttributeRam = &memory.GetSpriteAttributeRam();
	_ioPorts = &memory.GetIOPorts();
}

void Renderer::SetInterruptHandler(InterruptHandler &interruptHandler)
{
	_interruptHandler = &interruptHandler;
}

Memory &Renderer::GetMemory()
{
	return *_memory;
}

InterruptHandler &Renderer::GetInterruptHandler()
{
	return *_interruptHandler;
}
