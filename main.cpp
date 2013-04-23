#include <iostream>
#include <fstream>	
#include <chrono>
#include <unistd.h>
#include "core/GameboyColor.hpp"
#include "ui/GameWindow.hpp"

int main(int argc, char *argv[])
{
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
	
		while (window.isOpen())
		{
			window.Render();
		}
		
		delete arrayRom;
	}
	else
	{
		printf("HAHAHA FEHLER!!!");
	}
	
	return 0;
}
