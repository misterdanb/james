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
			Scanline(Color[]);
			~Scanline();
			
			Color *GetGameboyColors();
			Color *GetRealColors();
			
			int GetLength();
			
		private:
			Color *_gameboyColors;
			Color *_realColors;
		};
	}
}

#endif
