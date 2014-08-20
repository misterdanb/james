#ifndef _TileMapWindow_hpp_
#define _TileMapWindow_hpp_

#include <SFML/Graphics.hpp>
#include "../core/GameboyColor.hpp"
#include "../core/Color.tpp"

namespace james
{
	namespace sfml
	{
		using namespace core;

		class TileMapWindow : public sf::RenderWindow
		{
		  public:
			TileMapWindow (int, GameboyColor&);
			~TileMapWindow();

			void Render();

		  private:
			int _tileMapNumber;
			GameboyColor& _gbc;

			sf::Image _frame;
			Array<sf::Uint8, core::TileMap::WIDTH* core::Tile::WIDTH* core::TileMap::HEIGHT* core::Tile::HEIGHT * 4> _rawFrame;
		};
	}
}

#endif
