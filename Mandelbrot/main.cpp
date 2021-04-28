#include <SFML/Graphics.hpp>

#include "farm.h"
#include "task.h"
#include "mandelbrot_task.h"
#include "mandelbrot.h"

// 3 / 2.25 = ASPECT RATIO
#define MB_RECT -2.0, 1.0, 1.125, -1.125

sf::RenderWindow* window;
Mandelbrot* mandelbrot;

void runFarm() {
	Farm farm;

	const double num_segments = 200.0;
	const double slice = (double)height / num_segments;

	for (int i = 0; i < num_segments; i++) {
		farm.add_task(new MandelbrotTask(mandelbrot->getImage(), MB_RECT, i * slice, i * slice + slice));
	}

	farm.run();
	mandelbrot->update();
}

int main()
{
	window = new sf::RenderWindow(sf::VideoMode(width, height), "Mandelbrot");
	mandelbrot = new Mandelbrot(width, height);

	runFarm();

	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();

			if (event.type == sf::Event::KeyPressed)
			{
				//handle_keyboard_input();
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				//handle_mouse_input();
			}
		}

		window->clear();
		window->draw(*mandelbrot);
		window->display();
	}

	return 0;
}