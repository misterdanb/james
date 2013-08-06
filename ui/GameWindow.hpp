#ifndef _GameWindow_hpp_
#define _GameWindow_hpp_

#include <SFML/Graphics.hpp>
#include "../core/GameboyColor.hpp"
#include "../core/LCD.hpp"
#include "../core/Joypad.hpp"
#include "../core/Color.template"

#include "TileMapWindow.hpp"

namespace gbc
{
	namespace ui
	{
		using namespace core;
		
		class GameWindow : public sf::RenderWindow, public ILCD, public IJoypad
		{
		public:
			GameWindow(int, int, DynamicArray<int> &);
			~GameWindow();
			
			void Render();
			
			void DrawFrame(Frame &);
			
			int GetRight();
			int GetLeft();
			int GetUp();
			int GetDown();
			int GetButtonA();
			int GetButtonB();
			int GetSelect();
			int GetStart();
			
			void ShowTileMap(int);
			void HideTileMap(int);
		
		private:
			void throwTilemaps();
			void throwMem();
			void throwRegs();
			void throwSprAttr();
			
			core::GameboyColor _gbc;
			
			Array<sf::Uint8, Frame::WIDTH * Frame::HEIGHT * 4> _rawFrame;
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
			int _tileMap0WindowVisible;
			int _tileMap1WindowVisible;
			
			TileMapWindow _tileMap0Window;
			TileMapWindow _tileMap1Window;
#endif
		};
	}
}

#endif
