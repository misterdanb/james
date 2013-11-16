#include "Tile.hpp"

using namespace james;
using namespace james::core;

Tile::Tile()
	: ByteMap<WIDTH, HEIGHT>()
{
}

Tile::Tile(Array<int, DATA_SIZE> tileData)
	: ByteMap<WIDTH, HEIGHT>()
{
	SetData(tileData);
}

Tile::~Tile()
{
}

void Tile::SetData(Array<int, DATA_SIZE> tileData)
{
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			_matrixElements[x][y] = (((tileData[y] >> (7 - x)) & 0x01) << 1) |
			                        ((tileData[y + 1] >> (7 - x)) & 0x01);
		}
	}
}

Array<int, Tile::DATA_SIZE> Tile::GetData()
{
	Array<int, DATA_SIZE> tileData;
	
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			tileData[y] |= (((_matrixElements[x][y] >> 1) & 0x01) << (7 - x));
			tileData[y + 1] |= (_matrixElements[x][y] & 0x01) << (7 - x);
		}
	}
	
	return tileData;
}
