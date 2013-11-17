#include "Renderer.hpp"

Renderer::Renderer() : _memory(nullptr),
		       _interruptHandler(nullptr),
		       _videoRam(nullptr),
		       _spriteAttributeRam(nullptr),
		       _ioPorts(nullptr)
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
