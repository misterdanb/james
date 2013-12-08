#include "TileViewerWindow.hpp"

using namespace james;
using namespace james::sfml;

TileViewerWindow::TileViewerWindow(core::Emulator &emulator)
	: sf::RenderWindow(sf::VideoMode(core::Tile::WIDTH,
	                                 core::Tile::HEIGHT),
	                   std::string("TileViewer - ") + ToDec(0x0000)),
	  _tileIndex(0),
	  _emulator(emulator),
	  _frame(),
	  _rawFrame()
{
}

TileViewerWindow::~TileViewerWindow()
{
}

void TileViewerWindow::Render()
{
	sf::Event event;
	sf::Texture texture;

    core::Tile tile = _emulator.GetMemory().GetVideoRam().GetTile(_tileIndex);
	
	for (int y = 0; y < core::Tile::HEIGHT; y++)
	{
		for (int x = 0; x < core::Tile::WIDTH; x++)
		{
			int pixelIndex = y * core::Tile::WIDTH + x;

			_rawFrame[pixelIndex * 4] = (sf::Uint8) (tile.GetArray()[x][y] * 0x1F);
			_rawFrame[pixelIndex * 4 + 1] = (sf::Uint8) (tile.GetArray()[x][y] * 0x1F);
			_rawFrame[pixelIndex * 4 + 2] = (sf::Uint8) (tile.GetArray()[x][y] * 0x1F);
			_rawFrame[pixelIndex * 4 + 3] = (sf::Uint8) (0xFF);
		}
	}
	
	_frame.create(core::Tile::WIDTH,
                  core::Tile::HEIGHT,
                  &_rawFrame[0]);
	
	texture.loadFromImage(_frame,
                          sf::IntRect(0,
                                      0,
                                      core::Tile::WIDTH,
                                      core::Tile::HEIGHT));
	
	sf::Sprite sprite(texture);
	
	while (pollEvent(event))
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                _tileIndex--;

                if (_tileIndex < 0)
                {
                    _tileIndex = 0;
                }

                setTitle(std::string("TileViewer - ") + ToDec(_tileIndex));
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                _tileIndex++;

                if (_tileIndex >= 384)
                {
                    _tileIndex = 383;
                }
                
                setTitle(std::string("TileViewer - ") + ToDec(_tileIndex));
            }
        }
	}
	
	clear();
	draw(sprite);
	display();
}
