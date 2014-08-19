#ifndef _Tile_hpp_
#define _Tile_hpp_

#include "James.hpp"

using namespace james;
using namespace james::core;

namespace james
{
	namespace core
	{
		class Tile : public Array2<int, 8, 8>
		{
		public:
			static const int WIDTH = 8;
			static const int HEIGHT = 8;
			
			static const int DATA_SIZE = 16;
			
			typedef Array2<int, WIDTH, HEIGHT> TileArray2;
		
		public:
			Tile();
			Tile(Array<int, DATA_SIZE>);
			~Tile();
			
			void SetData(Array<int, DATA_SIZE>);
			
			Array<int, DATA_SIZE> GetData();
		};
	}
}

#endif
