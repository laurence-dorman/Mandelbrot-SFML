#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#include "farm.h"
#include "task.h"
#include "mandelbrot_task.h"
#include "mandelbrot.h"

sf::RenderWindow* window;
Mandelbrot* mandelbrot;

// -0.678027 -0.674145 0.363877 0.360249
// -0.603531 -0.488737 0.65485 0.568753
// -0.756145 -0.709464 0.260511 0.223836

double l = -2.25;
double r = 0.75;
double t = 1.5;
double b = -1.5;

double view_width = std::abs(r-l);
double view_height = std::abs(t-b);

int max_iterations = 100;

void runFarm() {

	Farm farm;
	const double num_segments = 200.0;
	const double slice = (double)height / num_segments;

	for (int i = 0; i < num_segments; i++) {
		farm.add_task(new MandelbrotTask(mandelbrot->getImage(), l, r, t, b, i * slice, i * slice + slice, max_iterations));
	}

	farm.run();
	mandelbrot->update();
	std::cout << l <<  " " << r << " " << t << " " << b << std::endl;
}

void updateViewSize() {
	// update view size variables since these will change everytime we move/zoom
	view_width = std::abs(r - l);
	view_height = std::abs(t - b);
}

void lerpToPos(double x, double y, double m_t, double z_t){

	// x, y is the center of the new view rectangle, m_t is the movement lerp "time" (0-1), z_t is the zoom lerp "time" (0-1), z_t can be negative to zoom out.

	// move to new view rectangle (lerp the edges toward their new edge centered around x, y, based on m_t)
	l = std::lerp(l, x - view_width / 2, m_t);
	r = std::lerp(r, x + view_width / 2, m_t);

	t = std::lerp(t, y + view_height / 2, m_t);
	b = std::lerp(b, y - view_height / 2, m_t);

	// zoom in (lerp the edges toward eachother based on z_t)
	l = std::lerp(l, r, z_t);
	r = std::lerp(r, l, z_t);

	t = std::lerp(t, b, z_t);
	b = std::lerp(b, t, z_t);

	updateViewSize();

	runFarm();

	max_iterations += 50 * z_t; // increase detail (max_iterations) as we zoom in, and decrease as we zoom out
}

void handleMouseInput() {
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		lerpToPos(sf::Mouse::getPosition(*window).x / (double)width * view_width + l, sf::Mouse::getPosition(*window).y / (double)height * -view_height + t, 1.0, 0.1);
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		lerpToPos(sf::Mouse::getPosition(*window).x / (double)width * view_width + l, sf::Mouse::getPosition(*window).y / (double)height * -view_height + t, 1.0, -0.1);
	}
}

int main()
{
	window = new sf::RenderWindow(sf::VideoMode(width, height), "Mandelbrot");
	mandelbrot = new Mandelbrot(width, height);

	runFarm();

	sf::CircleShape xhair;
	xhair.setFillColor(sf::Color::Green);
	xhair.setRadius(1.0f);
	xhair.setPosition(((float)width / 2.f) - xhair.getGlobalBounds().width / 2.f, ((float)height / 2.f) - xhair.getGlobalBounds().height / 2.f);


	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();

			if (event.type == sf::Event::KeyPressed)
			{
				
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				handleMouseInput();
			}
		}

		window->clear();
		window->draw(*mandelbrot);
		window->draw(xhair);
		window->display();
	}

	return 0;
}