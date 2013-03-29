#ifndef _GameWindow_hpp_
#define _GameWindow_hpp_

#include <SFML/Graphics.hpp>

namespace gbc
{
	namespace ui
	{
		class GameWindow : public sf::RenderWindow
		{
		public:
			GameWindow(int, int);
			~GameWindow();
			
			void Render();
		};
	}
}

#endif
