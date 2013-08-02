#include "RomDimensions.hpp"

using namespace gbc;
using namespace gbc::core;
using namespace gbc::core::cartridges;

RomDimensions::RomDimensions(int romDimensionsIdentifier)
: size(), banks(), bankSize()
{
	switch (romDimensionsIdentifier)
	{
		case 0x00:
			size = 32 * 1024;
			banks = 2;
			bankSize = 16 * 1024;
			break;
		
		case 0x01:
			size = 64 * 1024;
			banks = 4;
			bankSize = 16 * 1024;
			break;
		
		case 0x02:
			size = 128 * 1024;
			banks = 8;
			bankSize = 16 * 1024;
			break;
		
		case 0x03:
			size = 256 * 1024;
			banks = 16;
			bankSize = 16 * 1024;
			break;
		
		case 0x04:
			size = 512 * 1024;
			banks = 32;
			bankSize = 16 * 1024;
			break;
		
		case 0x05:
			size = 1 * 1024 * 1024;
			banks = 64;
			bankSize = 16 * 1024;
			break;
		
		case 0x06:
			size = 2 * 1024 * 1024;
			banks = 128;
			bankSize = 16 * 1024;
			break;
		
		case 0x07:
			size = 4 * 1024 * 1024;
			banks = 256;
			bankSize = 16 * 1024;
			break;
		
		case 0x52:
			size = 72 * 16 * 1024;
			banks = 72;
			bankSize = 16 * 1024;
			break;
		
		case 0x53:
			size = 80 * 16 * 1024;
			banks = 80;
			bankSize = 16 * 1024;
			break;
		
		case 0x54:
			size = 96 * 16 * 1024;
			banks = 96;
			bankSize = 16 * 1024;
			break;
		
		default: break;
	}
}

RomDimensions::~RomDimensions()
{
}
