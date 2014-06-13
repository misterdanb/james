#include "GameWindow.hpp"

using namespace james;
using namespace james::core;
using namespace james::sfml;

GameWindow::GameWindow (int width, int height, std::string rom)
	: sf::RenderWindow (sf::VideoMode (width, height), "James"),
	  _rawFrame(),
	  _frame(),
	  _rightPressed (GBC_FALSE),
	  _leftPressed (GBC_FALSE),
	  _upPressed (GBC_FALSE),
	  _downPressed (GBC_FALSE),
	  _buttonAPressed (GBC_FALSE),
	  _buttonBPressed (GBC_FALSE),
	  _selectPressed (GBC_FALSE),
	  _startPressed (GBC_FALSE)
	  #ifdef DEBUG
	  ,_tileMap0WindowVisible (GBC_FALSE),
	  _tileMap1WindowVisible (GBC_FALSE),
	  _tileMap0Window (0, GetDevice()),
	  _tileMap1Window (1, GetDevice())
	  #endif
{
	#ifdef DEBUG
	_tileMap0Window.setVisible (GBC_FALSE);
	_tileMap1Window.setVisible (GBC_FALSE);
	#endif
	LoadRom (rom);

	Initialize();
	Start();
}

GameWindow::~GameWindow()
{
}

void GameWindow::Render()
{
	RenderFrame();

	sf::Event event;
	sf::Texture texture;

	texture.loadFromImage (_frame, sf::IntRect (0, 0, 160, 144));

	sf::Sprite sprite (texture);
	//sprite.setScale(2, 2);

	while (pollEvent (event))
	{
		if (event.type == sf::Event::Closed)
		{
			Pause();
			Finalize();

			LOG ("Exiting James");

			close();
		}

		else if (event.type == sf::Event::KeyPressed)
		{
			if (sf::Keyboard::isKeyPressed (sf::Keyboard::LControl))
			{
				if (sf::Keyboard::isKeyPressed (sf::Keyboard::R))
				{
					Reset();
				}

				if (sf::Keyboard::isKeyPressed (sf::Keyboard::P))
				{
					if (IsPaused())
					{
						Start();
					}
					else
					{
						Pause();
					}
				}

				if (sf::Keyboard::isKeyPressed (sf::Keyboard::Z))
				{
					if (!GetDevice().GetProcessor().IsRecording())
					{
						GetDevice().GetProcessor().StartRecording ("cpu.record");
					}
					else
					{
						GetDevice().GetProcessor().StopRecording();
					}
				}

				if (sf::Keyboard::isKeyPressed (sf::Keyboard::T))
				{
					LOG ("Current tilemaps");

					std::cout << std::endl;

					ThrowTilemaps();
				}

				if (sf::Keyboard::isKeyPressed (sf::Keyboard::M))
				{
					LOG ("Read from memory");

					std::cout << std::endl;

					unsigned int address = 0;

					std::cout << "Read memory location (hex value [!]): 0x";
					std::cin >> std::hex >> address;
					std::cout << "MEM[0x" << ToFixedHex (address, 4) << "] = "
							  << "0x" << ToFixedHex (GetDevice().ReadByte (address), 2) << std::endl << std::endl;
				}

				if (sf::Keyboard::isKeyPressed (sf::Keyboard::D))
				{
					LOG ("Current memory");

					std::cout << std::endl;

					ThrowMem();
				}

				if (sf::Keyboard::isKeyPressed (sf::Keyboard::A))
				{
					LOG ("Current cpu registers");

					ThrowRegs();
					ThrowSprAttr();
				}
			}
		}

		bool irqMark = false;

		if (_rightPressed = sf::Keyboard::isKeyPressed (sf::Keyboard::Right))
		{
			irqMark = true;
		}

		if (_leftPressed = sf::Keyboard::isKeyPressed (sf::Keyboard::Left))
		{
			irqMark = true;
		}

		if (_upPressed = sf::Keyboard::isKeyPressed (sf::Keyboard::Up))
		{
			irqMark = true;
		}

		if (_downPressed = sf::Keyboard::isKeyPressed (sf::Keyboard::Down))
		{
			irqMark = true;
		}

		if (_buttonAPressed = sf::Keyboard::isKeyPressed (sf::Keyboard::Period))
		{
			irqMark = true;
		}

		if (_buttonBPressed = sf::Keyboard::isKeyPressed (sf::Keyboard::Comma))
		{
			irqMark = true;
		}

		if (_selectPressed = sf::Keyboard::isKeyPressed (sf::Keyboard::Dash))
		{
			irqMark = true;
		}

		if (_startPressed = sf::Keyboard::isKeyPressed (sf::Keyboard::Space))
		{
			irqMark = true;
		}

		if (irqMark)
		{
			GetDevice().GetInterruptHandler().SignalJoypadInterrupt();
		}
	}

	clear();
	draw (sprite);
	display();

	#ifdef DEBUG

	if (_tileMap0WindowVisible)
	{
		_tileMap0Window.Render();
	}

	if (_tileMap1WindowVisible)
	{
		_tileMap1Window.Render();
	}

	#endif
}

void GameWindow::DrawFrame (Frame& frame)
{
	core::Frame::FrameArray2& rawMap = frame.data;

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

	_frame.create (Frame::WIDTH, Frame::HEIGHT, &_rawFrame[0]);
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

#ifdef DEBUG
void GameWindow::ShowTileMap (int tileMapNumber)
{
	if (tileMapNumber == 0)
	{
		_tileMap0WindowVisible = true;
		_tileMap0Window.setVisible (_tileMap0WindowVisible);
	}
	else if (tileMapNumber == 1)
	{
		_tileMap1WindowVisible = true;
		_tileMap1Window.setVisible (_tileMap1WindowVisible);
	}
}

void GameWindow::HideTileMap (int tileMapNumber)
{
	if (tileMapNumber == 0)
	{
		_tileMap0WindowVisible = GBC_FALSE;
		_tileMap0Window.setVisible (_tileMap0WindowVisible);
	}
	else if (tileMapNumber == 1)
	{
		_tileMap1WindowVisible = GBC_FALSE;
		_tileMap1Window.setVisible (_tileMap1WindowVisible);
	}

	_tileMap1Window.setVisible (false);
	_tileMap0Window.setVisible (false);
}
#endif

void GameWindow::ThrowTilemaps()
{
	for (int i = 0; i < 2; i++)
	{
		TileMap::TileMapArray2 tileMap = GetDevice().GetTileMap (i);

		std::cout << "TILEMAP " << ToDec (i) << std::endl;

		std::cout << "     ";

		for (int x = 0; x < TileMap::WIDTH; x++)
		{
			std::cout << " " << ToFixedDec (x, 2);
		}

		std::cout << std::endl;

		std::cout << "-----";

		for (int x = 0; x < TileMap::WIDTH; x++)
		{
			std::cout << "---";
		}

		std::cout << std::endl;

		for (int y = 0; y < TileMap::HEIGHT; y++)
		{
			std::cout << ToFixedDec (y, 2) << " | ";

			for (int x = 0; x < TileMap::WIDTH; x++)
			{
				std::cout << " " << ToFixedHex (tileMap[x][y], 2);
			}

			std::cout << std::endl;
		}

		std::cout << std::endl;
	}

}

void GameWindow::ThrowMem()
{
	for (int i = 0; i < 0x10000; i++)
	{
		if ((i % 0x0100) == 0)
		{
			std::cout << std::endl << std::endl;;

			std::cout << ToFixedHex ((i >> 8) & 0xFF, 2) << "** |";

			for (int j = 0; j < 0x10; j++)
			{
				std::cout << " " << ToFixedHex ((i >> 8) & 0xFF, 2) << "*" << ToFixedHex (j, 1);
			}

			std::cout << std::endl;

			std::cout << "------";

			for (int j = 0; j < 0x10; j++)
			{
				std::cout << "-----";
			}
		}

		if ((i % 0x10) == 0)
		{
			std::cout << std::endl << ToFixedHex ((i >> 4) & 0xFFF, 3) << "* |";
		}

		std::cout << "   " << ToFixedHex (GetDevice().ReadByte (i), 2);
	}

	std::cout << std::endl << std::endl;
}

void GameWindow::ThrowRegs()
{
	core::cpu::State state = GetDevice().GetProcessor().GetState();

	std::cout << "\tA = 0x" << ToFixedHex (state.a, 2) << std::endl;
	std::cout << "\tF = 0x" << ToFixedHex (state.f, 2) << std::endl;
	std::cout << "\tB = 0x" << ToFixedHex (state.b, 2) << std::endl;
	std::cout << "\tC = 0x" << ToFixedHex (state.c, 2) << std::endl;
	std::cout << "\tD = 0x" << ToFixedHex (state.d, 2) << std::endl;
	std::cout << "\tE = 0x" << ToFixedHex (state.e, 2) << std::endl;
	std::cout << "\tH = 0x" << ToFixedHex (state.h, 2) << std::endl;
	std::cout << "\tL = 0x" << ToFixedHex (state.l, 2) << std::endl;
	std::cout << "\tSP = 0x" << ToFixedHex (state.pc, 2) << std::endl;
	std::cout << "\tPC = 0x" << ToFixedHex (state.sp, 2) << std::endl;
	std::cout << "\tIME = " << std::boolalpha << bool (state.interruptsEnabled) << std::endl;
	std::cout << "\tStopped = " << std::boolalpha << bool (state.stopped) << std::endl;
	std::cout << "\tHalted = " << std::boolalpha << bool (state.halted) << std::endl;

	std::cout << std::endl;

}

void GameWindow::ThrowSprAttr()
{
	std::cout << std::endl;

	for (int i = 0; i < 40; i++)
	{
		core::SpriteAttribute spriteAttribute = GetDevice().GetSpriteAttribute (i);

		std::cout << "\t Y = " << spriteAttribute.y << std::endl;
		std::cout << "\t X = " << spriteAttribute.x << std::endl;
		std::cout << "\t Tile number = " << spriteAttribute.tileNumber << std::endl;
		std::cout << "\t Color palette number = " << spriteAttribute.colorPaletteNumber << std::endl;
		std::cout << "\t Tile video ram bank number = " << spriteAttribute.tileVideoRamBankNumber << std::endl;
		std::cout << "\t Monochrome palette number = " << spriteAttribute.monochromePaletteNumber << std::endl;
		std::cout << "\t Horizontal flip = " << GetEnumValue (spriteAttribute.horizontalFlip) << std::endl;
		std::cout << "\t Vertical flip = " << GetEnumValue (spriteAttribute.verticalFlip) << std::endl;
		std::cout << "\t Sprite to background priority = " << GetEnumValue (spriteAttribute.spriteToBackgroundPriority) << std::endl;

		std::cout << std::endl;
	}
}
