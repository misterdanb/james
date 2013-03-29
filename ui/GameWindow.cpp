#include "GameWindow.hpp"

gbc::ui::GameWindow::GameWindow(int width, int height)
	: sf::RenderWindow(sf::VideoMode(width, height), "GBC")
{
	FinishFrame();
}

gbc::ui::GameWindow::~GameWindow()
{
}

void gbc::ui::GameWindow::Render()
{FinishFrame();
	sf::Event event;
	sf::Texture texture;
	
	texture.loadFromImage(_frame, sf::IntRect(0, 0, 160, 144));
	
	sf::Sprite sprite(texture);
	
	while (pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			close();
		}
	}
	
	clear();
	draw(sprite);
	display();
}

void gbc::ui::GameWindow::DrawScanline(core::Scanline scanline)
{
	if (scanline.GetLineIndex() < 144)
	{
		_frameBuffer[scanline.GetLineIndex()] = scanline;
	}
}

void gbc::ui::GameWindow::FinishFrame()
{
	sf::Uint8 rawData[144 * 160 * 4];
	
	for (int i = 0; i < 144; i++)
	{
		for (int j = 0; j < 160; j++)
		{
			int offset = (i * 160 + j) * 4;
			
			rawData[offset] = (sf::Uint8) (_frameBuffer[i].GetRealColors()[j].red);
			rawData[offset + 1] = (sf::Uint8) (_frameBuffer[i].GetRealColors()[j].green);
			rawData[offset + 2] = (sf::Uint8) (_frameBuffer[i].GetRealColors()[j].blue);
			rawData[offset + 3] = (sf::Uint8) (0x59); // test
		}
	}
	
	_frame.create(160, 144, rawData);
	
	/*for (int i = 0; i < 144 * 160 * 4; i++)
	{
		delete &rawData[i];
	}*/
}
