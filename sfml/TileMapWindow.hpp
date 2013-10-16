#ifndef _TileMapWindow_hpp_
#define _TileMapWindow_hpp_

#include <SFML/Graphics.hpp>
#include "../core/Emulator.hpp"
#include "../core/Color.template"

namespace james
{
	namespace sfml
	{
		using namespace core;
		
		class TileMapWindow : public sf::RenderWindow
		{
		public:
			TileMapWindow(int, Emulator &);
			~TileMapWindow();
			
			void Render();
		
		private:
			int _tileMapNumber;
			Emulator &_gbc;
			
			sf::Image _frame;
			Array<sf::Uint8, core::BackgroundTileNumberMap::WIDTH * core::Tile::WIDTH * core::BackgroundTileNumberMap::HEIGHT * core::Tile::HEIGHT * 4> _rawFrame;
		};
	}
}

#endif
