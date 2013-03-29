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
			virtual ~ILCD(); 
			virtual void DrawScanline(Scanline);
			virtual void FinishFrame();
		};
	}
}

#endif
