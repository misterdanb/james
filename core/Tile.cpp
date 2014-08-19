#include "Tile.hpp"

using namespace james;
using namespace james::core;

Tile::Tile()
{
}

Tile::Tile(Array<int, DATA_SIZE> tileData)
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
			(*this)[x][y] = (((tileData[y * 2] >> (7 - x)) & 0x01) << 1) |
			                ((tileData[y * 2 + 1] >> (7 - x)) & 0x01);
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
			tileData[y * 2] |= (((*this)[x][y] >> 1) & 0x01) << (7 - x);
			tileData[y * 2 + 1] |= ((*this)[x][y] & 0x01) << (7 - x);
		}
	}
	
	return tileData;
}
