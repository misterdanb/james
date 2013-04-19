#include "GameWindow.hpp"

gbc::ui::GameWindow::GameWindow(int width, int height, int rom[], int size)
	: sf::RenderWindow(sf::VideoMode(width, height), "GBC"),
	  _rightPressed(GBC_FALSE),
	  _leftPressed(GBC_FALSE),
	  _upPressed(GBC_FALSE),
	  _downPressed(GBC_FALSE),
	  _buttonAPressed(GBC_FALSE),
	  _buttonBPressed(GBC_FALSE),
	  _selectPressed(GBC_FALSE),
	  _startPressed(GBC_FALSE)
{
	_gbc.Initialize();
	_gbc.SetRom(rom, size);
	_gbc.SetLCD(this);
	_gbc.SetJoypad(this);
}

gbc::ui::GameWindow::~GameWindow()
{
}

void gbc::ui::GameWindow::Render()
{
	_gbc.RenderFrame();
	
	sf::Event event;
	sf::Texture texture;
	
	texture.loadFromImage(_frame, sf::IntRect(0, 0, 160, 144));
	
	sf::Sprite sprite(texture);
	//sprite.setScale(2, 2);
	
	while (pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			close();
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			_rightPressed = GBC_TRUE;
			_gbc.GetInterruptHandler()->SignalJoypadInterrupt();
		}
		else
		{
			_rightPressed = GBC_FALSE;
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			_leftPressed = GBC_TRUE;
			_gbc.GetInterruptHandler()->SignalJoypadInterrupt();
		}
		else
		{
			_leftPressed = GBC_FALSE;
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			_upPressed = GBC_TRUE;
			_gbc.GetInterruptHandler()->SignalJoypadInterrupt();
		}
		else
		{
			_upPressed = GBC_FALSE;
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			_downPressed = GBC_TRUE;
			_gbc.GetInterruptHandler()->SignalJoypadInterrupt();
		}
		else
		{
			_downPressed = GBC_FALSE;
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Period))
		{
			_buttonAPressed = GBC_TRUE;
			_gbc.GetInterruptHandler()->SignalJoypadInterrupt();
		}
		else
		{
			_buttonAPressed = GBC_FALSE;
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Comma))
		{
			_buttonBPressed = GBC_TRUE;
			_gbc.GetInterruptHandler()->SignalJoypadInterrupt();
		}
		else
		{
			_buttonBPressed = GBC_FALSE;
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Dash))
		{
			_selectPressed = GBC_TRUE;
			_gbc.GetInterruptHandler()->SignalJoypadInterrupt();
		}
		else
		{
			_selectPressed = GBC_FALSE;
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			_startPressed = GBC_TRUE;
			_gbc.GetInterruptHandler()->SignalJoypadInterrupt();
		}
		else
		{
			_startPressed = GBC_FALSE;
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
		core::Color *realColors = scanline.GetRealColors();
		
		for (int i = 0; i < 160; i++)
		{
			int lineIndex = scanline.GetLineIndex();
			int lineOffset = lineIndex * 160;
			
			_rawFrame[(lineOffset + i) * 4] = (sf::Uint8) (realColors[i].red);
			_rawFrame[(lineOffset + i) * 4 + 1] = (sf::Uint8) (realColors[i].green);
			_rawFrame[(lineOffset + i) * 4 + 2] = (sf::Uint8) (realColors[i].blue);
			_rawFrame[(lineOffset + i) * 4 + 3] = (sf::Uint8) (0xFF); // test
		}
	}
}

void gbc::ui::GameWindow::FinishFrame()
{
	_frame.create(160, 144, _rawFrame);
	static int i = 0;
	printf("%d\n", i);
	i++;
	/*for (int i = 0; i < 144 * 160 * 4; i++)
	{
		delete &rawData[i];
	}*/
}

int gbc::ui::GameWindow::GetRight()
{
	return _rightPressed;
}

int gbc::ui::GameWindow::GetLeft()
{
	return _leftPressed;
}

int gbc::ui::GameWindow::GetUp()
{
	return _upPressed;
}

int gbc::ui::GameWindow::GetDown()
{
	return _downPressed;
}

int gbc::ui::GameWindow::GetButtonA()
{
	return _buttonAPressed;
}

int gbc::ui::GameWindow::GetButtonB()
{
	return _buttonBPressed;
}

int gbc::ui::GameWindow::GetSelect()
{
	return _selectPressed;
}

int gbc::ui::GameWindow::GetStart()
{
	return _startPressed;
}
