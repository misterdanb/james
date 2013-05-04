#include "GameWindow.hpp"

gbc::ui::GameWindow::GameWindow(int width, int height, DynamicArray<int> &rom)
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
	_gbc.SetRom(rom);
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

void gbc::ui::GameWindow::DrawFrame(core::Frame frame)
{
	core::Frame::FrameArray2 &rawMap = frame.data;
	
	for (int y = 0; y < core::Frame::HEIGHT; y++)
	{
		for (int x = 0; x < core::Frame::WIDTH; x++)
		{
			int pixelIndex = y * core::Frame::WIDTH + x;
			
			_rawFrame[pixelIndex * 4] = (sf::Uint8) (rawMap[x][y].red);
			_rawFrame[pixelIndex * 4 + 1] = (sf::Uint8) (rawMap[x][y].green);
			_rawFrame[pixelIndex * 4 + 2] = (sf::Uint8) (rawMap[x][y].blue);
			_rawFrame[pixelIndex * 4 + 3] = (sf::Uint8) (0xFF);
		}
	}
	
	_frame.create(core::Frame::WIDTH, core::Frame::HEIGHT, _rawFrame);
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
