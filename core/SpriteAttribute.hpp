#ifndef _SpriteAttribute_hpp_
#define _SpriteAttribute_hpp_

#include "SpriteToBackgroundPriority.hpp"

namespace gbc
{
	namespace core
	{
		class SpriteAttribute
		{
		public:
			int y = 0;
			int x = 0;
			int tileNumber = 0;
			int spriteColorPaletteNumber = 0;
			int tileVideoRamBankNumber = 0;
			int spriteMonochromePaletteNumber = 0;
			HorizontalFlip horizontalFlip = HorizontalFlip(0);
			VerticalFlip verticalFlip = VerticalFlip(0);
			SpriteToBackgroundPriority spriteToBackgroundPriority = SpriteToBackgroundPriority(0);
		};
	}
}

#endif
