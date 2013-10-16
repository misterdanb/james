#include "VideoRam.hpp"

using namespace james;
using namespace james::core;

VideoRam::VideoRam()
	: MemoryBlock(VIDEO_RAM_BANKS, VIDEO_RAM_BANK_SIZE)
{
}

VideoRam::~VideoRam()
{
}

int VideoRam::ReadByte(int address)
{
	return ReadByteFromBank((*_ioPorts).GetSelectedVideoRamBank(), address - VIDEO_RAM_OFFSET);
}

void VideoRam::WriteByte(int address, int value)
{
	WriteByteToBank((*_ioPorts).GetSelectedVideoRamBank(), address - VIDEO_RAM_OFFSET, value);
}

void VideoRam::SetIOPorts(IOPorts &ioPorts)
{
	_ioPorts = &ioPorts;
}

void VideoRam::SetTile(int tileIndex, Tile tile)
{
	Array<int, Tile::DATA_SIZE> tileData = tile.GetData();
	
	for (int address = tileIndex * Tile::DATA_SIZE, i = 0; i < Tile::DATA_SIZE; address++, i++)
	{
		WriteByteToBank(0, address, tileData[i]);
	}
}

void VideoRam::SetBackgroundTileNumber(int backgroundIndex, int backgroundTileNumberIndex, int backgroundTileNumber)
{
	WriteByteToBank(0, 0x9800 + backgroundIndex * BackgroundTileNumberMap::DATA_SIZE + backgroundTileNumberIndex, backgroundTileNumber);
}

void VideoRam::SetBackgroundTileNumberMap(int backgroundIndex, BackgroundTileNumberMap backgroundTileNumberMap)
{
	Array<int, BackgroundTileNumberMap::DATA_SIZE> backgroundTileNumberMapData = backgroundTileNumberMap.GetData();
	
	for (int address = 0x9800 + backgroundIndex * BackgroundTileNumberMap::DATA_SIZE, i = 0; i < BackgroundTileNumberMap::DATA_SIZE; address++, i++)
	{
		WriteByteToBank(0, address, backgroundTileNumberMapData[i]);
	}
}

void VideoRam::SetBackgroundAttribute(int backgroundIndex, int backgroundAttributeIndex, BackgroundAttribute backgroundAttribute)
{
	WriteByteToBank(1, 0x9800 + backgroundIndex * BackgroundAttributeMap::DATA_SIZE + backgroundAttributeIndex, backgroundAttribute.GetData());
}

void VideoRam::SetBackgroundAttributeMap(int index, BackgroundAttributeMap backgroundAttributeMap)
{
	Array<int, BackgroundAttributeMap::DATA_SIZE> backgroundAttributeMapData = backgroundAttributeMap.GetData();
	
	for (int address = 0x9800 + index * BackgroundAttributeMap::DATA_SIZE, i = 0; i < BackgroundAttributeMap::DATA_SIZE; address++, i++)
	{
		WriteByteToBank(1, address, backgroundAttributeMapData[i]);
	}
}

IOPorts &VideoRam::GetIOPorts()
{
	return *_ioPorts;
}

Tile VideoRam::GetTile(int tileIndex)
{
	Array<int, Tile::DATA_SIZE> tileData;
	
	for (int address = tileIndex * Tile::DATA_SIZE, i = 0; i < Tile::DATA_SIZE; address++, i++)
	{
		tileData[i] = ReadByteFromBank(0, address);
	}
	
	return Tile(tileData);
}

int VideoRam::GetBackgroundTileNumber(int backgroundIndex, int backgroundTileNumberIndex)
{
	return ReadByteFromBank(0, 0x9800 + backgroundIndex * BackgroundTileNumberMap::DATA_SIZE + backgroundTileNumberIndex);
}

BackgroundTileNumberMap VideoRam::GetBackgroundTileNumberMap(int backgroundIndex)
{
	Array<int, BackgroundTileNumberMap::DATA_SIZE> backgroundTileNumberMapData;
	
	for (int address = 0x9800 + backgroundIndex * BackgroundTileNumberMap::DATA_SIZE, i = 0; i < BackgroundTileNumberMap::DATA_SIZE; address++, i++)
	{
		backgroundTileNumberMapData[i] = ReadByteFromBank(0, address);
	}
	
	return BackgroundTileNumberMap(backgroundTileNumberMapData);
}

BackgroundAttribute VideoRam::GetBackgroundAttribute(int backgroundIndex, int backgroundAttributeIndex)
{
	return ReadByteFromBank(1, 0x9800 + backgroundIndex * BackgroundAttributeMap::DATA_SIZE + backgroundAttributeIndex);
}

BackgroundAttributeMap VideoRam::GetBackgroundAttributeMap(int index)
{
	Array<int, BackgroundAttributeMap::DATA_SIZE> backgroundAttributeMapData;
	
	for (int address = 0x9800 + index * BackgroundAttributeMap::DATA_SIZE, i = 0; i < BackgroundAttributeMap::DATA_SIZE; address++, i++)
	{
		backgroundAttributeMapData[i] = ReadByteFromBank(1, address);
	}
}
