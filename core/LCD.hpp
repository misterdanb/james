#ifndef _LCD_hpp_
#define _LCD_hpp_

#include "GBC.hpp"
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
			
			virtual void DrawScanline(Scanline) = GBC_NULL;
			virtual void FinishFrame() = GBC_NULL;
		};
	}
}

#endif
