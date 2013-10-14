#include "BackgroundAttributeMap.hpp"

using namespace james;
using namespace james::core;

BackgroundAttributeMap::BackgroundAttributeMap()
	: ByteMap<WIDTH, HEIGHT>()
{
}

BackgroundAttributeMap::BackgroundAttributeMap(Array<int, BackgroundAttributeMap::DATA_SIZE> backgroundAttributeMapData)
	: ByteMap<WIDTH, HEIGHT>()
{
	SetData(backgroundAttributeMapData);
}

BackgroundAttributeMap::~BackgroundAttributeMap()
{
}

void BackgroundAttributeMap::SetData(Array<int, BackgroundAttributeMap::DATA_SIZE> backgroundAttributeMapData)
{
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			_matrixElements[x][y] = backgroundAttributeMapData[y * WIDTH + x];
		}
	}
}

void BackgroundAttributeMap::SetBackgroundAttribute(int x, int y, BackgroundAttribute backgroundAttribute)
{
	_matrixElements[x][y] = backgroundAttribute.GetData();
}

Array<int, BackgroundAttributeMap::DATA_SIZE> BackgroundAttributeMap::GetData()
{
	Array<int, WIDTH * HEIGHT> backgroundAttributeMapData;
	
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			backgroundAttributeMapData[y * WIDTH + x] = _matrixElements[x][y];
		}
	}
	
	return backgroundAttributeMapData;
}

BackgroundAttribute BackgroundAttributeMap::GetBackgroundAttribute(int x, int y)
{
	return BackgroundAttribute(_matrixElements[x][y]);
}
