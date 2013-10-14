#ifndef _BackgroundTileNumberMap_hpp_
#define _BackgroundTileNumberMap_hpp_

#include "James.hpp"

namespace james
{
	namespace core
	{
		class BackgroundTileNumberMap : public ByteMap<32, 32>
		{
		public:
			static const int WIDTH = 32;
			static const int HEIGHT = 32;
			
			static const int DATA_SIZE = WIDTH * HEIGHT;
			
		public:
			BackgroundTileNumberMap();
			BackgroundTileNumberMap(Array<int, DATA_SIZE>);
			~BackgroundTileNumberMap();
			
			void SetData(Array<int, DATA_SIZE>);
			void SetTileNumber(int, int, int);
			
			Array<int, DATA_SIZE> GetData();
			int GetTileNumber(int, int);
			
		public:
			using ByteMap<WIDTH, HEIGHT>::_matrixElements;
		};
	}
}

#endif
