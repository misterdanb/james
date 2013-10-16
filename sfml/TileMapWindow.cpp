#include "TileMapWindow.hpp"

using namespace james;
using namespace james::sfml;

TileMapWindow::TileMapWindow(int tileMapNumber, core::Emulator &gbc)
	: sf::RenderWindow(sf::VideoMode(core::BackgroundTileNumberMap::WIDTH * core::Tile::WIDTH,
	                                 core::BackgroundTileNumberMap::HEIGHT * core::Tile::HEIGHT),
	                   std::string("TILEMAP") + ToDec(tileMapNumber)),
	  _tileMapNumber(tileMapNumber),
	  _gbc(gbc),
	  _frame(),
	  _rawFrame()
{
}

TileMapWindow::~TileMapWindow()
{
}

void TileMapWindow::Render()
{
	/*sf::Event event;
	sf::Texture texture;
	
	auto rendererTileMap = _gbc.GetRenderer().GetRenderedTileMap(_tileMapNumber);
	
	int tileMapPixelWidth = core::BackgroundTileNumberMap::WIDTH * core::Tile::WIDTH;
	int tileMapPixelHeight = core::BackgroundTileNumberMap::HEIGHT * core::Tile::HEIGHT;
	
	for (int y = 0; y < tileMapPixelWidth; y++)
	{
		for (int x = 0; x < tileMapPixelHeight; x++)
		{
			int pixelIndex = y * tileMapPixelWidth + x;
			
			_rawFrame[pixelIndex * 4] = (sf::Uint8) (rendererTileMap.GetArray()[x][y].GetRed());
			_rawFrame[pixelIndex * 4 + 1] = (sf::Uint8) (rendererTileMap.GetArray()[x][y].GetGreen());
			_rawFrame[pixelIndex * 4 + 2] = (sf::Uint8) (rendererTileMap.GetArray()[x][y].getBlue());
			_rawFrame[pixelIndex * 4 + 3] = (sf::Uint8) (0xFF);
		}
	}
	
	_frame.create(tileMapPixelWidth, tileMapPixelHeight, &_rawFrame[0]);
	
	texture.loadFromImage(_frame, sf::IntRect(0, 0, tileMapPixelWidth, tileMapPixelHeight));
	
	sf::Sprite sprite(texture);
	
	while (pollEvent(event))
	{
	}
	
	clear();
	draw(sprite);
	display();*/
}
