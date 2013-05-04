#include <iostream>
#include <fstream>	
#include <chrono>
#include <unistd.h>
#include "core/GameboyColor.hpp"
#include "ui/GameWindow.hpp"
#include "core/Vector2.template"

int main(int argc, char *argv[])
{
	gbc::core::Vector2<float> a(1.2, 2.3);
	gbc::core::RenderContext rc;
	rc.tiles[0][0].data[0][0] = 123;
	gbc::core::Tile test = rc.tiles[0][0];
	std::cout << "tile " << test.data[0][0] << std::endl;
	std::cout << "(" << a.x << ", " << a.y << ")" << std::endl;
	
	/*typedef std::chrono::high_resolution_clock Clock;
	typedef std::chrono::milliseconds milliseconds;
	
	Clock::time_point t0 = Clock::now();
	
	gbc::core::GameboyColor myGBC = gbc::core::GameboyColor();
	myGBC.Initialize();
	
	Clock::time_point t1 = Clock::now();
	
	milliseconds ms = std::chrono::duration_cast<milliseconds>(t1 - t0);
	std::cout << ms.count() << "ms\n";*/
	
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
			rom.push_back((int) byte);
		}
		
		file.close();
		
		int *arrayRom = new int[rom.size()];
		
		for (int i = 0; i < rom.size(); i++)
		{
			arrayRom[i] = rom[i] & 0xFF;
		}
		
		/*gbc::core::GameboyColor gbc;
		gbc.SetRom(arrayRom, rom.size());
		gbc.RenderFrame();*/
		
		gbc::ui::GameWindow window(160, 144, arrayRom, rom.size());
		
		sf::Clock clock;
		float lastTime = 0;
		float frames = 0;
		
		while (window.isOpen())
		{
			window.Render();
			
			float currentTime = clock.getElapsedTime().asSeconds();
			float fps = frames / currentTime;
			//lastTime = currentTime;
			
			std::cout << fps << std::endl;
			
			frames++;
		}
		
		delete arrayRom;
	}
	else
	{
		printf("HAHAHA FEHLER!!!");
	}
	
	return 0;
}
