#ifndef _ColorPalette_hpp_
#define _ColorPalette_hpp_

#include "Color.hpp"

namespace gbc
{
	namespace core
	{
		class ColorPalette
		{
		public:
			static const int COLOR_PALETTE_SIZE = 4;
		
		public:
			Color colors[COLOR_PALETTE_SIZE];
		};
	}
}

#endif
