#include "Tile.hpp"

Tile::Tile()
	: ByteMap<WIDTH, HEIGHT>(), data (_matrixElements)
{
}

Tile::~Tile()
{
}
