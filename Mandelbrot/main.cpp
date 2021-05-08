#include <SFML/Graphics.hpp>
#include <iostream>

#include "fractal_manager.h"
#include "utils.h"

inline void printControls() {
	std::cout << "CONTROLS:\n" << std::endl;
	std::cout << "- *Mouse left click* to zoom in on click location.\n" << std::endl;
	std::cout << "- *Mouse right click* to zoom out on click location.\n" << std::endl;
	std::cout << "- *Right and left arrow keys* to change colour theme.\n" << std::endl;
	std::cout << "- *Scroll wheel up/down* to increase/decrease maximum iterations.\n" << std::endl;
	std::cout << "- *L* to initiate animation, then console keyboard input to set it up. If you wish to zoom to mouse location, place mouse on desired location first, then press 'L' and then enter '1' when you are prompted to use mouse location. A directory will be created called 'output', the animation frames will be saved to this new directory.\n" << std::endl;
	std::cout << "- *R* to reset to initial position and zoom.\n" << std::endl;
	std::cout << "- *1, 2* to switch between Mandelbrot and Multibrot (negative exponent) mode respectively.\n" << std::endl;
	std::cout << "- *C* to print controls to console.\n\n" << std::endl;
}

int main()
{
	printControls();

	double size = 0.0;
	std::cout << "Enter scale % of window (0-100)\n>> ";
	std::cin >> size;
	size /= 100;

	std::cout << std::endl;

	unsigned int width = sf::VideoMode::getDesktopMode().width * size;
	unsigned int height = sf::VideoMode::getDesktopMode().height * size;

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
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
				{
					printControls();
				}
				else 
				{
					fractalManager.handleKeyboardInput();
				}
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