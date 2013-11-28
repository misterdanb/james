#include "VideoRam.hpp"

using namespace james;
using namespace james::core;

VideoRam::VideoRam()
	: MemoryBlock(VIDEO_RAM_BANKS,
	  VIDEO_RAM_BANK_SIZE),
	  _ioPorts(nullptr)
{
}

VideoRam::~VideoRam()
{
}

int VideoRam::ReadByte(int address)
{
	return ReadByteFromBank((*_ioPorts).GetSelectedVideoRamBank(),
	                        address - VIDEO_RAM_OFFSET);
}

void VideoRam::WriteByte(int address,
                         int value)
{
	WriteByteToBank((*_ioPorts).GetSelectedVideoRamBank(),
	                address - VIDEO_RAM_OFFSET,
	                value);
}

void VideoRam::SetIOPorts(IOPorts &ioPorts)
{
	_ioPorts = &ioPorts;
}

void VideoRam::SetTile(int  tileIndex,
                       Tile tile)
{
	Array<int, 
	      Tile::DATA_SIZE>
	      tileData = tile.GetData();
	
	int address = tileIndex * Tile::DATA_SIZE;

	for (int i = 0; i < Tile::DATA_SIZE; i++)
	{
		WriteByteToBank(0,
		                address++,
		                tileData[i]);
	}
}

void VideoRam::SetBackgroundTileNumber(int backgroundIndex,
                                       int backgroundTileNumberIndex,
                                       int backgroundTileNumber)
{
	int address = 0x1800
	            + backgroundIndex * BackgroundTileNumberMap::DATA_SIZE
	            + backgroundTileNumberIndex;

	WriteByteToBank(0,
	                address,
	                backgroundTileNumber);
}

void VideoRam::SetBackgroundTileNumberMap(int backgroundIndex,
                                          BackgroundTileNumberMap backgroundTileNumberMap)
{
	Array<int,
	      BackgroundTileNumberMap::DATA_SIZE>
	      backgroundTileNumberMapData = backgroundTileNumberMap.GetData();
	
	int address = 0x1800
	        + backgroundIndex * BackgroundTileNumberMap::DATA_SIZE;

	for (int i = 0; i < BackgroundTileNumberMap::DATA_SIZE; i++)
	{
		WriteByteToBank(0,
		                address++,
		                backgroundTileNumberMapData[i]);
	}
}

void VideoRam::SetBackgroundAttribute(int backgroundIndex,
                                      int backgroundAttributeIndex,
                                      BackgroundAttribute backgroundAttribute)
{

	int address = 0x1800
	            + backgroundIndex * BackgroundAttributeMap::DATA_SIZE
	            + backgroundAttributeIndex;

	WriteByteToBank(1,
	                address,
	                backgroundAttribute.GetData());
}

void VideoRam::SetBackgroundAttributeMap(int index,
                                         BackgroundAttributeMap backgroundAttributeMap)
{
	Array<int,
	      BackgroundAttributeMap::DATA_SIZE>
	      backgroundAttributeMapData = backgroundAttributeMap.GetData();
	
	int address = 0x1800
	            + index * BackgroundAttributeMap::DATA_SIZE;

	for (int i = 0; i < BackgroundAttributeMap::DATA_SIZE; i++)
	{
		WriteByteToBank(1,
		                address++,
		                backgroundAttributeMapData[i]);
	}
}

IOPorts &VideoRam::GetIOPorts()
{
	return *_ioPorts;
}

Tile VideoRam::GetTile(int tileIndex)
{
	Array<int,
	      Tile::DATA_SIZE>
	      tileData;
	
	int address = tileIndex * Tile::DATA_SIZE;

	for (int i = 0; i < Tile::DATA_SIZE; i++)
	{
		tileData[i] = ReadByteFromBank(0,
		                               address++);
	}
	
	return Tile(tileData);
}

int VideoRam::GetBackgroundTileNumber(int backgroundIndex,
                                      int backgroundTileNumberIndex)
{
	int address = 0x1800
	            + backgroundIndex * BackgroundTileNumberMap::DATA_SIZE
	            + backgroundTileNumberIndex;
	return ReadByteFromBank(0,
	                        address);
}

BackgroundTileNumberMap VideoRam::GetBackgroundTileNumberMap(int backgroundIndex)
{
	Array<int,
	      BackgroundTileNumberMap::DATA_SIZE>
	      backgroundTileNumberMapData;

	int address = 0x1800
	            + backgroundIndex * BackgroundTileNumberMap::DATA_SIZE;

	for (int i = 0; i < BackgroundTileNumberMap::DATA_SIZE; i++)
	{
		backgroundTileNumberMapData[i] = ReadByteFromBank(0,
		                                                  address++);
	}
	
	return BackgroundTileNumberMap(backgroundTileNumberMapData);
}

BackgroundAttribute VideoRam::GetBackgroundAttribute(int backgroundIndex,
                                                     int backgroundAttributeIndex)
{
	int address = 0x1800
	            + backgroundIndex * BackgroundAttributeMap::DATA_SIZE
	            + backgroundAttributeIndex;

	return ReadByteFromBank(1,
	                        address);
}

BackgroundAttributeMap VideoRam::GetBackgroundAttributeMap(int index)
{
	Array<int,
	      BackgroundAttributeMap::DATA_SIZE>
	      backgroundAttributeMapData;
	
	int address = 0x1800
	            + index * BackgroundAttributeMap::DATA_SIZE;

	for (int i = 0; i < BackgroundAttributeMap::DATA_SIZE; i++)
	{
		backgroundAttributeMapData[i] = ReadByteFromBank(1,
		                                                 address++);
	}
	return backgroundAttributeMapData;
}
