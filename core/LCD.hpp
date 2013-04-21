#ifndef _LCD_hpp_
#define _LCD_hpp_

#include "Scanline.hpp"

namespace gbc
{
	namespace core
	{
		class ILCD
		{
		public:
			static const int LCD_WIDTH = Scanline::WIDTH;
			static const int LCD_HEIGHT = 144;
		
		public:
			virtual ~ILCD();
			
			virtual void DrawScanline(Scanline) = 0;
			virtual void FinishFrame() = 0;
		};
	}
}

#endif
