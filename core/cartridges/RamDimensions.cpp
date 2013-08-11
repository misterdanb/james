#include "RamDimensions.hpp"

using namespace james;
using namespace james::core;
using namespace james::core::cartridges;

RamDimensions::RamDimensions(int ramDimensionsIdentifier)
{
	switch (ramDimensionsIdentifier)
	{
		case 0x00:
			size = 0;
			banks = 0;
			bankSize = 0;
			break;
		
		case 0x01:
			size = 2 * 1024;
			banks = 1;
			bankSize = 2 * 1024;
			break;
		
		case 0x02:
			size = 8 * 1024;
			banks = 1;
			bankSize = 8 * 1024;
			break;
		
		case 0x03:
			size = 32 * 1024;
			banks = 4;
			bankSize = 8 * 1024;
			break;
		
		default: break;
	}
}

RamDimensions::~RamDimensions()
{
}
