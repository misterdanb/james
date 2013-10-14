#ifndef _LCD_hpp_
#define _LCD_hpp_

#include "James.hpp"
#include "Frame.hpp"

namespace james
{
	namespace core
	{
		/*
		 * LCD is an interface to the physical presentation unit to
		 * show the rendered frames.
		 */
		
		class LCD
		{
		public:
			static const int LCD_WIDTH = Frame::WIDTH;
			static const int LCD_HEIGHT = Frame::HEIGHT;
		
		public:
			virtual ~LCD();
			
			virtual void DrawFrame(Frame) = GBC_NULL;
		};
	}
}

#endif
