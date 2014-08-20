#ifndef _SpriteAttribute_hpp_
#define _SpriteAttribute_hpp_

#include "James.hpp"
#include "SpriteToBackgroundPriority.hpp"
#include "HorizontalFlip.hpp"
#include "VerticalFlip.hpp"

namespace james
{
	namespace core
	{
		class SpriteAttribute
		{
		public:
			SpriteAttribute();
			SpriteAttribute(Array<int, 4>);
			~SpriteAttribute();
			
			void SetData(Array<int, 4>);
			
			void SetX(int);
			void SetY(int);
			void SetTileNumber(int);
			void SetColorPaletteNumber(int);
			void SetTileVideoRamBankNumber(int);
			void SetMonochromePaletteNumber(int);
			void SetHorizontalFlip(HorizontalFlip);
			void SetVerticalFlip(VerticalFlip);
			void SetSpriteToBackgroundPriority(SpriteToBackgroundPriority);
			
			Array<int, 4> GetData() const;
			
			int GetX() const;
			int GetY() const;
			int GetTileNumber() const;
			int GetColorPaletteNumber() const;
			int GetTileVideoRamBankNumber() const;
			int GetMonochromePaletteNumber() const;
			HorizontalFlip GetHorizontalFlip() const;
			VerticalFlip GetVerticalFlip() const;
			SpriteToBackgroundPriority GetSpriteToBackgroundPriority() const;
			
		private:
			int _y;
			int _x;
			int _tileNumber;
			int _colorPaletteNumber;
			int _tileVideoRamBankNumber;
			int _monochromePaletteNumber;
			HorizontalFlip _horizontalFlip;
			VerticalFlip _verticalFlip;
			SpriteToBackgroundPriority _spriteToBackgroundPriority;
		};
	}
}

#endif
