#include <SFML/Graphics.hpp>
#include <iostream>

#include "farm.h"
#include "task.h"
#include "mandelbrot_task.h"
#include "mandelbrot.h"

sf::RenderWindow* window;
Mandelbrot* mandelbrot;

double l =	-2.25;
double r =	 0.75;
double t =	 1.5;
double b =  -1.5;

double wh_scale = (double)width / (double)height;

void runFarm() {
	Farm farm;

	const double num_segments = 960.0;
	const double slice = (double)height / num_segments;

	for (int i = 0; i < num_segments; i++) {
		farm.add_task(new MandelbrotTask(mandelbrot->getImage(), l, r, t, b, i * slice, i * slice + slice));
	}

	farm.run();
	mandelbrot->update();
	std::cout << l <<  " " << r << " " << t << " " << b << std::endl;
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
				// keyboard input
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