#ifndef _TileViewerWindow_hpp_
#define _TileViewerWindow_hpp_

#include <SFML/Graphics.hpp>
#include "../core/Emulator.hpp"
#include "../core/Color.template"

namespace james
{
	namespace sfml
	{
		using namespace core;
		
		class TileViewerWindow : public sf::RenderWindow
		{
		public:
			TileViewerWindow(Emulator &);
			~TileViewerWindow();
			
			void Render();
		
		private:
			int _tileIndex;
			Emulator &_emulator;
			
            int _scaleFactor;

			sf::Image _frame;
			Array<sf::Uint8, core::Tile::WIDTH * core::Tile::HEIGHT * 4> _rawFrame;
		};
	}
}

#endif
