#ifndef _BackgroundAttribute_hpp_
#define _BackgroundAttribute_hpp_

#include "HorizontalFlip.hpp"
#include "VerticalFlip.hpp"
#include "BackgroundToOAMPriority.hpp"

struct BackgroundMapAttribute
{
	int backgroundColorPaletteNumber;
	int tileVideoRamBankNumber;
	HorizontalFlip horizontalFlip;
	VerticalFlip verticalFlip;
	BackgroundToOAMPriority backgroundToOAMPriority;
};

#endif
