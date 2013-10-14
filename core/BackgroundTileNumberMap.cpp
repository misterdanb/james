#include "BackgroundTileNumberMap.hpp"

using namespace james;
using namespace james::core;

BackgroundTileNumberMap::BackgroundTileNumberMap()
	: ByteMap<WIDTH, HEIGHT>()
{
}

BackgroundTileNumberMap::BackgroundTileNumberMap(Array<int, BackgroundTileNumberMap::DATA_SIZE> backgroundTileNumberMapData)
	: ByteMap<WIDTH, HEIGHT>()
{
	SetData(backgroundTileNumberMapData);
}

BackgroundTileNumberMap::~BackgroundTileNumberMap()
{
}

void BackgroundTileNumberMap::SetData(Array<int, BackgroundTileNumberMap::DATA_SIZE> backgroundTileNumberMapData)
{
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			_matrixElements[x][y] = backgroundTileNumberMapData[y * WIDTH + x];
		}
	}
}

void BackgroundTileNumberMap::SetTileNumber(int x, int y, int number)
{
	_matrixElements[x][y] = number;
}

Array<int, BackgroundTileNumberMap::DATA_SIZE> BackgroundTileNumberMap::GetData()
{
	Array<int, WIDTH * HEIGHT> backgroundTileNumberMapData;
	
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			backgroundTileNumberMapData[y * WIDTH + x] = _matrixElements[x][y];
		}
	}
	
	return backgroundTileNumberMapData;
}

int BackgroundTileNumberMap::GetTileNumber(int x, int y)
{
	return _matrixElements[x][y];
}
