#include "SpriteAttributeRam.hpp"

using namespace james;
using namespace james::core;

SpriteAttributeRam::SpriteAttributeRam()
	: MemoryBlock(SPRITE_ATTRIBUTE_RAM_BANKS, SPRITE_ATTRIBUTE_RAM_BANK_SIZE)
{
}

SpriteAttributeRam::~SpriteAttributeRam()
{
}

int SpriteAttributeRam::ReadByte(int address)
{
	return ReadByteFromBank(0, address);
}

void SpriteAttributeRam::WriteByte(int address, int value)
{
	WriteByteToBank(0, address, value);
}

void SpriteAttributeRam::SetSpriteAttribute(int index, SpriteAttribute spriteAttribute)
{
	int ramAddress = index * 4;
	
	Array<int, 4> spriteAttributeData = spriteAttribute.GetData();
	
	WriteByteToBank(0, ramAddress, spriteAttributeData[0]);
	WriteByteToBank(0, ramAddress + 1, spriteAttributeData[1]);
	WriteByteToBank(0, ramAddress + 2, spriteAttributeData[2]);
	WriteByteToBank(0, ramAddress + 3, spriteAttributeData[3]);
}

SpriteAttribute SpriteAttributeRam::GetSpriteAttribute(int spriteAttributeIndex)
{
	int ramAddress = spriteAttributeIndex * 4;
	
	Array<int, 4> spriteAttributeData = { ReadByteFromBank(0, ramAddress),
	                                      ReadByteFromBank(0, ramAddress + 1), 
	                                      ReadByteFromBank(0, ramAddress + 2), 
	                                      ReadByteFromBank(0, ramAddress + 3) };
	
	return SpriteAttribute(spriteAttributeData);
}
