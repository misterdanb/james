#ifndef _BackgroundAttribute_hpp_
#define _BackgroundAttribute_hpp_

#include "James.hpp"
#include "HorizontalFlip.hpp"
#include "VerticalFlip.hpp"
#include "BackgroundToOAMPriority.hpp"

namespace james
{
	namespace core
	{
		class BackgroundAttribute
		{
		public:
			BackgroundAttribute();
			BackgroundAttribute(int);
			~BackgroundAttribute();
			
			void SetData(int);
			
			void SetBackgroundColorPaletteNumber(int);
			void SetTileVideoRamBankNumber(int);
			void SetHorizontalFlip(HorizontalFlip);
			void SetVerticalFlip(VerticalFlip);
			void SetBackgroundToOAMPriority(BackgroundToOAMPriority);
			
			int GetData();
			
			int &GetBackgroundColorPaletteNumber();
			int &GetTileVideoRamBankNumber();
			HorizontalFlip &GetHorizontalFlip();
			VerticalFlip &GetVerticalFlip();
			BackgroundToOAMPriority &GetBackgroundToOAMPriority();
			
		private:
			int _backgroundColorPaletteNumber;
			int _tileVideoRamBankNumber;
			HorizontalFlip _horizontalFlip;
			VerticalFlip _verticalFlip;
			BackgroundToOAMPriority _backgroundToOAMPriority;
		};
	}
}

#endif
