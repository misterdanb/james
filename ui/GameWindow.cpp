#include "GameWindow.hpp"

gbc::ui::GameWindow::GameWindow(int width, int height)
	: sf::RenderWindow(sf::VideoMode(width, height), "GBC")
{
}

gbc::ui::GameWindow::~GameWindow()
{
}

void gbc::ui::GameWindow::Render()
{
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);
	
	sf::Event event;
	
	while (pollEvent(event))
	{
		if (event.type == sf::Event::Closed) close();
	}
	
	clear();
	draw(shape);
	display();
}
