#include "GameWindow.hpp"

using namespace gbc;
using namespace gbc::core;
using namespace gbc::ui;

GameWindow::GameWindow(int width, int height, DynamicArray<int> &rom)
	: sf::RenderWindow(sf::VideoMode(width, height), "GBC"),
	  _rightPressed(GBC_FALSE),
	  _leftPressed(GBC_FALSE),
	  _upPressed(GBC_FALSE),
	  _downPressed(GBC_FALSE),
	  _buttonAPressed(GBC_FALSE),
	  _buttonBPressed(GBC_FALSE),
	  _selectPressed(GBC_FALSE),
	  _startPressed(GBC_FALSE),
	  _tileMap0WindowVisible(GBC_FALSE),
	  _tileMap1WindowVisible(GBC_FALSE),
	  _tileMap0Window(0, _gbc),
	  _tileMap1Window(1, _gbc)
{
	_tileMap0Window.setVisible(GBC_FALSE);
	_tileMap1Window.setVisible(GBC_FALSE);
	
	_gbc.Initialize();
	_gbc.SetRom(rom);
	_gbc.SetLCD(*this);
	_gbc.SetJoypad(*this);
}

GameWindow::~GameWindow()
{
}

void GameWindow::Render()
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
			_gbc.Finalize();
			
			LOG("Exiting James");
			
			close();
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			_rightPressed = GBC_TRUE;
			_gbc.GetInterruptHandler().SignalJoypadInterrupt();
		}
		else
		{
			_rightPressed = GBC_FALSE;
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			_leftPressed = GBC_TRUE;
			_gbc.GetInterruptHandler().SignalJoypadInterrupt();
		}
		else
		{
			_leftPressed = GBC_FALSE;
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			_upPressed = GBC_TRUE;
			_gbc.GetInterruptHandler().SignalJoypadInterrupt();
		}
		else
		{
			_upPressed = GBC_FALSE;
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			_downPressed = GBC_TRUE;
			_gbc.GetInterruptHandler().SignalJoypadInterrupt();
		}
		else
		{
			_downPressed = GBC_FALSE;
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Period))
		{
			_buttonAPressed = GBC_TRUE;
			_gbc.GetInterruptHandler().SignalJoypadInterrupt();
		}
		else
		{
			_buttonAPressed = GBC_FALSE;
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Comma))
		{
			_buttonBPressed = GBC_TRUE;
			_gbc.GetInterruptHandler().SignalJoypadInterrupt();
		}
		else
		{
			_buttonBPressed = GBC_FALSE;
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Dash))
		{
			_selectPressed = GBC_TRUE;
			_gbc.GetInterruptHandler().SignalJoypadInterrupt();
		}
		else
		{
			_selectPressed = GBC_FALSE;
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			_startPressed = GBC_TRUE;
			_gbc.GetInterruptHandler().SignalJoypadInterrupt();
		}
		else
		{
			_startPressed = GBC_FALSE;
		}
	}
	
	clear();
	draw(sprite);
	display();
	
	if (_tileMap0WindowVisible)
	{
		_tileMap0Window.Render();
	}
	
	if (_tileMap1WindowVisible)
	{
		_tileMap1Window.Render();
	}
}

void GameWindow::DrawFrame(Frame &frame)
{
	core::Frame::FrameArray2 &rawMap = frame.data;
	
	for (int y = 0; y < Frame::HEIGHT; y++)
	{
		for (int x = 0; x < Frame::WIDTH; x++)
		{
			int pixelIndex = y * Frame::WIDTH + x;
			
			_rawFrame[pixelIndex * 4] = (sf::Uint8) (rawMap[x][y].red);
			_rawFrame[pixelIndex * 4 + 1] = (sf::Uint8) (rawMap[x][y].green);
			_rawFrame[pixelIndex * 4 + 2] = (sf::Uint8) (rawMap[x][y].blue);
			_rawFrame[pixelIndex * 4 + 3] = (sf::Uint8) (0xFF);
		}
	}
	
	_frame.create(Frame::WIDTH, Frame::HEIGHT, &_rawFrame[0]);
}

int GameWindow::GetRight()
{
	return _rightPressed;
}

int GameWindow::GetLeft()
{
	return _leftPressed;
}

int GameWindow::GetUp()
{
	return _upPressed;
}

int GameWindow::GetDown()
{
	return _downPressed;
}

int GameWindow::GetButtonA()
{
	return _buttonAPressed;
}

int GameWindow::GetButtonB()
{
	return _buttonBPressed;
}

int GameWindow::GetSelect()
{
	return _selectPressed;
}

int GameWindow::GetStart()
{
	return _startPressed;
}

void GameWindow::ShowTileMap(int tileMapNumber)
{
	if (tileMapNumber == 0)
	{
		_tileMap0WindowVisible = GBC_TRUE;
		_tileMap0Window.setVisible(_tileMap0WindowVisible);
	}
	else if (tileMapNumber == 1)
	{
		_tileMap1WindowVisible = GBC_TRUE;
		_tileMap1Window.setVisible(_tileMap1WindowVisible);
	}
}

void GameWindow::HideTileMap(int tileMapNumber)
{
	if (tileMapNumber == 0)
	{
		_tileMap0WindowVisible = GBC_FALSE;
		_tileMap0Window.setVisible(_tileMap0WindowVisible);
	}
	else if (tileMapNumber == 1)
	{
		_tileMap1WindowVisible = GBC_FALSE;
		_tileMap1Window.setVisible(_tileMap1WindowVisible);
	}
	
	_tileMap1Window.setVisible(false);
	_tileMap0Window.setVisible(false);
}
