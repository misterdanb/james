#include <iostream>
#include <chrono>
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
	
	gbc::ui::GameWindow *window = new gbc::ui::GameWindow(160, 144);
	
	while (window->isOpen())
	{
		window->Render();
	}
	
	delete window;
	
	return 0;
}
