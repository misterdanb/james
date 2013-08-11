#ifndef _ColorPalette_hpp_
#define _ColorPalette_hpp_

#include "James.hpp"

namespace james
{
	namespace core
	{
		class ColorPalette : public Vector<Color<int>, 4>
		{
		public:
			static const int COLOR_PALETTE_SIZE = 4;
			
			typedef Array<Color<int>, COLOR_PALETTE_SIZE> ColorPaletteArray;
		
		public:
			ColorPalette()
				: Vector<Color<int>, COLOR_PALETTE_SIZE>(), colors(_vectorElements)
			{
			}
			
			~ColorPalette()
			{
			}
		
		public:
			ColorPaletteArray &colors;
		
		protected:
			using Vector<Color<int>, COLOR_PALETTE_SIZE>::_vectorElements;
		};
	}
}

#endif
