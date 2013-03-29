#ifndef _GameWindow_hpp_
#define _GameWindow_hpp_

#include <SFML/Graphics.hpp>
#include "../core/LCD.hpp"

namespace gbc
{
	namespace ui
	{
		class GameWindow : public sf::RenderWindow, public core::ILCD
		{
		public:
			GameWindow(int, int);
			~GameWindow();
			
			void Render();
			
			void DrawScanline(core::Scanline);
			void FinishFrame();
		
		private:
			core::Scanline _frameBuffer[144];
			sf::Image _frame;
		};
	}
}

#endif
