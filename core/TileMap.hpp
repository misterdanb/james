#ifndef _TileMap_hpp_
#define _TileMap_hpp_

#include "gbc.hpp"

namespace gbc
{
	namespace core
	{
		class TileMap : public ByteMap<32, 32>
		{
		public:
			static const int WIDTH = 32;
			static const int HEIGHT = 32;
			
			typedef Array2<int, WIDTH, HEIGHT> TileMapArray2;
		
		public:
			TileMap()
				: ByteMap<WIDTH, HEIGHT>(), data(_matrixElements)
			{
			}
			
			~TileMap()
			{
			}
		
		public:
			TileMapArray2 &data;
		
		public:
			using ByteMap<WIDTH, HEIGHT>::_matrixElements;
		};
	}
}

#endif
