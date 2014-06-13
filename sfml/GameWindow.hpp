#ifndef _GameWindow_hpp_
#define _GameWindow_hpp_

#include <SFML/Graphics.hpp>
#include "../core/Frontend.hpp"

#include "TileMapWindow.hpp"

namespace james
{
	namespace sfml
	{
		using namespace core;

		class GameWindow : public sf::RenderWindow, public Frontend
		{
		  public:
			GameWindow (int, int, std::string);
			~GameWindow();

			void Render();

			void DrawFrame (Frame&);

			int GetRight();
			int GetLeft();
			int GetUp();
			int GetDown();
			int GetButtonA();
			int GetButtonB();
			int GetSelect();
			int GetStart();

			void ShowTileMap (int);
			void HideTileMap (int);

		  private:
			void ThrowTilemaps();
			void ThrowMem();
			void ThrowRegs();
			void ThrowSprAttr();

			Array<sf::Uint8, Frame::WIDTH* Frame::HEIGHT * 4> _rawFrame;
			sf::Image _frame;

			int _rightPressed;
			int _leftPressed;
			int _upPressed;
			int _downPressed;
			int _buttonAPressed;
			int _buttonBPressed;
			int _selectPressed;
			int _startPressed;
			#ifdef DEBUG
			bool _tileMap0WindowVisible;
			bool _tileMap1WindowVisible;

			TileMapWindow _tileMap0Window;
			TileMapWindow _tileMap1Window;
			#endif
		};
	}
}

#endif
