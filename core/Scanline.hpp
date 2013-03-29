#ifndef _Scanline_hpp_
#define _Scanline_hpp_

#include "Color.hpp"

namespace gbc
{
	namespace core
	{
		class Scanline
		{
		public:
			Scanline();
			Scanline(int, Color[]);
			~Scanline();
			
			int GetLineIndex();
			
			Color *GetGameboyColors();
			Color *GetRealColors();
			
			int GetLength();
			
		private:
			int _lineIndex;
			Color *_gameboyColors;
			Color *_realColors;
		};
	}
}

#endif
