#ifndef _Tile_hpp_
#define _Tile_hpp_

#include "James.hpp"

using namespace james;
using namespace james::core;

namespace james
{
	namespace core
	{
		class Tile : public ByteMap<8, 8>
		{
		public:
			static const int WIDTH = 8;
			static const int HEIGHT = 8;
			
			typedef Array2<int, WIDTH, HEIGHT> TileArray2;
		
		public:
			Tile();
			~Tile();
		
		public:
			TileArray2 &data;
		
		protected:
			using ByteMap<WIDTH, HEIGHT>::_matrixElements;
		};
	}
}

#endif
