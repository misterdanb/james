#ifndef _SpriteAttribute_hpp_
#define _SpriteAttribute_hpp_

#include "SpriteToBackgroundPriority.hpp"

struct SpriteAttribute
{
	int y;
	int x;
	int tileNumber;
	int spriteColorPaletteNumber;
	int tileVideoRamBankNumber;
	int spriteMonochromePaletteNumber;
	HorizontalFlip horizontalFlip;
	VerticalFlip verticalFlip;
	SpriteToBackgroundPriority spriteToBackgroundPriority;
};

#endif
