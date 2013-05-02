#ifndef _TileMap_hpp_
#define _TileMap_hpp_

#include "Tile.hpp"

namespace gbc
{
	namespace core
	{
		class TileMap
		{
		public:
			static const int WIDTH = 32;
			static const int HEIGHT = 32;
		
		public:
			int data[WIDTH][HEIGHT] = { { 0, }, };
		};
	}
}

#endif
