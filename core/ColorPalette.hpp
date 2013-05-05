#ifndef _ColorPalette_hpp_
#define _ColorPalette_hpp_

#include "Vector.template"
#include "Color.hpp"

namespace gbc
{
	namespace core
	{
		class ColorPalette : public Vector<Color, 4>
		{
		public:
			static const int COLOR_PALETTE_SIZE = 4;
			
			typedef Array<Color, COLOR_PALETTE_SIZE> ColorPaletteArray;
		
		public:
			ColorPalette()
				: Vector<Color, COLOR_PALETTE_SIZE>(), colors(_vectorElements)
			{
			}
			
			~ColorPalette()
			{
			}
		
		public:
			ColorPaletteArray &colors;
		
		protected:
			using Vector<Color, COLOR_PALETTE_SIZE>::_vectorElements;
		};
	}
}

#endif
