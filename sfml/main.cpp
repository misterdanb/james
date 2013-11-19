#include <iostream>
#include <fstream>	
#include <chrono>
#include <unistd.h>
#include "../core/James.hpp"
#include "GameWindow.hpp"

int main(int argc, char *argv[])
{
	if (argc != 2) return -1;
	
	if (std::string(argv[1]) == std::string("help"))
	{
		std::cout << "Ctrl + P: Pause/Start" << std::endl
		          << "Ctrl + Z: Start/Stop recording cpu instructions" << std::endl
		          << "Ctrl + M: Read a single register" << std::endl
		          << "Ctrl + D: Dump the whole memory" << std::endl
		          << "Ctrl + A: Dump the processor state" << std::endl
		          << "Ctrl + S: Dump the sprite attributes" << std::endl;
	}
	else
	{
		std::string path(argv[1]);
		
		james::sfml::GameWindow window(160, 144, path);
		
		sf::Clock clock;
		float lastTime = 0;
		float frames = 0;
		
		window.ShowTileMap(0);
		window.ShowTileMap(1);
		
		while (window.isOpen())
		{
			window.Render();
			
			float currentTime = clock.getElapsedTime().asSeconds();
			float fps = frames / currentTime;
			
			std::cout << fps << std::endl;
			
			frames++;
		}
	}	
	
	return 0;
}
