#ifndef _BackgroundAttribute_hpp_
#define _BackgroundAttribute_hpp_

#include "HorizontalFlip.hpp"
#include "VerticalFlip.hpp"
#include "BackgroundToOAMPriority.hpp"

namespace gbc
{
	namespace core
	{
		class TileMapAttribute
		{
		public:
			int backgroundColorPaletteNumber = 0;
			int tileVideoRamBankNumber = 0;
			HorizontalFlip horizontalFlip = HorizontalFlip(0);
			VerticalFlip verticalFlip = VerticalFlip(0);
			BackgroundToOAMPriority backgroundToOAMPriority = BackgroundToOAMPriority(0);
		};
	}
}

#endif
