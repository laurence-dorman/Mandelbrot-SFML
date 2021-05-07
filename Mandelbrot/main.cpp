#include <SFML/Graphics.hpp>

#include "fractal_manager.h"

int main()
{
	// set window size to be 75% of screen size
	unsigned int width = sf::VideoMode::getDesktopMode().width * 0.75;
	unsigned int height = sf::VideoMode::getDesktopMode().height * 0.75;

	sf::RenderWindow window(sf::VideoMode(width, height), "Mandelbrot " + std::to_string(width) + "x" + std::to_string(height), sf::Style::Titlebar);

	FractalManager fractalManager(&window);

	fractalManager.runFarm();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			if (event.type == sf::Event::KeyPressed)
			{
				fractalManager.handleKeyboardInput();
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				fractalManager.handleMouseInput();
			}
			if (event.type == sf::Event::MouseWheelScrolled) 
			{
				fractalManager.handleScrollInput(event.mouseWheelScroll.delta);
			}
		}

		fractalManager.update();

		window.clear();
		window.draw(*fractalManager.getFractal());
		window.display();
	}

	return 0;
}