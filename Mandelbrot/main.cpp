#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>

#include "fractal_manager.h"

sf::RenderWindow* window;
FractalManager* fractalManager;

// set window size to be a square 85% of window height
unsigned int screen_height = sf::VideoMode::getDesktopMode().height*0.85;
unsigned int width = screen_height;
unsigned int height = screen_height;

int main()
{
	window = new sf::RenderWindow(sf::VideoMode(width, height), "Mandelbrot " + std::to_string(width) + "x" + std::to_string(height), sf::Style::Titlebar);

	fractalManager = new FractalManager(window);

	fractalManager->runFarm();

	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window->close();
			}
			if (event.type == sf::Event::KeyPressed)
			{
				fractalManager->handleKeyboardInput();
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				fractalManager->handleMouseInput();
			}
			if (event.type == sf::Event::MouseWheelScrolled) 
			{
				fractalManager->handleScrollInput(event.mouseWheelScroll.delta);
			}
		}

		fractalManager->update();

		window->clear();
		window->draw(*fractalManager->getFractal());
		window->display();
	}

	return 0;
}