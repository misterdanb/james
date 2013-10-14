#ifndef _Frame_hpp_
#define _Frame_hpp_

#include "James.hpp"

using namespace james;
using namespace james::core;

namespace james
{
	namespace core
	{
		class Frame : public PixelMap<160, 144>
		{
		public:
			static const int WIDTH = 160;
			static const int HEIGHT = 144;
			
		public:
			Frame();
			Frame(Array2<Color<int>, WIDTH, HEIGHT>);
			~Frame();
			
		protected:
			using PixelMap<WIDTH, HEIGHT>::_matrixElements;
		};
	}
}

#endif
