#ifndef _Frame_hpp_
#define _Frame_hpp_

#include "gbc.hpp"

using namespace gbc;
using namespace gbc::core;

namespace gbc
{
	namespace core
	{
		class Frame : public PixelMap<160, 144>
		{
		public:
			static const int WIDTH = 160;
			static const int HEIGHT = 144;
			
			typedef Array2<Color<int>, WIDTH, HEIGHT> FrameArray2;
		
		public:
			Frame();
			Frame(Color<int>[]);
			~Frame();
		
		public:
			FrameArray2 &data;
		
		protected:
			using PixelMap<WIDTH, HEIGHT>::_matrixElements;
		};
	}
}

#endif
