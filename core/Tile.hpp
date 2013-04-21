#ifndef _Tile_hpp_
#define _Tile_hpp_

namespace gbc
{
	namespace core
	{
		class Tile
		{
		public:
			static const int WIDTH = 8;
			static const int HEIGHT = 8;
		
		public:
			int data[WIDTH][HEIGHT] =
			{
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0 }
			};
		};
	}
}

#endif
