#ifndef _Frame_hpp_
#define _Frame_hpp_

#include "PixelMap.hpp"

namespace gbc
{
	namespace core
	{
		class Frame : public PixelMap
		{
		public:
			static const int WIDTH = 160;
			static const int HEIGHT = 144;
		
		public:
			Frame();
			Frame(Color[]);
			~Frame();
		};
	}
}

#endif
