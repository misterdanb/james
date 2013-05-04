#ifndef _GameWindow_hpp_
#define _GameWindow_hpp_

#include <SFML/Graphics.hpp>
#include "../core/GameboyColor.hpp"
#include "../core/LCD.hpp"
#include "../core/Joypad.hpp"
#include "../core/Color.hpp"

namespace gbc
{
	namespace ui
	{
		class GameWindow : public sf::RenderWindow, public core::ILCD, public core::IJoypad
		{
		public:
			GameWindow(int, int, int[], int);
			~GameWindow();
			
			void Render();
			
			void DrawFrame(core::Frame);
			
			int GetRight();
			int GetLeft();
			int GetUp();
			int GetDown();
			int GetButtonA();
			int GetButtonB();
			int GetSelect();
			int GetStart();
		
		private:
			core::GameboyColor _gbc;
			
			sf::Uint8 _rawFrame[core::Frame::WIDTH * core::Frame::HEIGHT * 4];
			sf::Image _frame;
			
			int _rightPressed;
			int _leftPressed;
			int _upPressed;
			int _downPressed;
			int _buttonAPressed;
			int _buttonBPressed;
			int _selectPressed;
			int _startPressed;
		};
	}
}

#endif
