#ifndef _PixelMap_hpp_
#define _PixelMap_hpp

#include "Color.hpp"

namespace gbc
{
	namespace core
	{
		class PixelMap
		{
		public:
			PixelMap(int, int);
			virtual ~PixelMap();
			
			int GetWidth();
			int GetHeight();
			
			Color GetPixel(int, int);
			Color *GetRawMap();
		
		protected:
			int _width;
			int _height;
			
			Color *_rawMap;
		};
	}
}

#endif
