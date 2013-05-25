#include <iostream>
#include <fstream>	
#include <chrono>
#include <unistd.h>
#include "core/GameboyColor.hpp"
#include "ui/GameWindow.hpp"
#include "core/Vector2.template"

int main(int argc, char *argv[])
{
	if (argc != 2) return -1;
	
	std::string path(argv[1]);
	
	std::cout << path << std::endl << std::endl;
	
	std::vector<int> rom;
	
	std::ifstream file(path, std::ios::in | std::ios::binary);
	
	if (file.is_open())
	{
		while (!file.eof())
		{
			char byte;
			
			file.get(byte);
			rom.push_back(((int) byte) & 0xFF);
		}
		
		file.close();
		
		gbc::ui::GameWindow window(160, 144, rom);
		
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
			//lastTime = currentTime;
			
			std::cout << fps << std::endl;
			
			frames++;
		}
	}
	else
	{
		printf("HAHAHA FEHLER!!!");
	}
	
	return 0;
}
