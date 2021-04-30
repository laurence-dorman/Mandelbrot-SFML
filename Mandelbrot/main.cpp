#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <iomanip>

#include "farm.h"
#include "task.h"
#include "mandelbrot_task.h"
#include "mandelbrot.h"

sf::RenderWindow* window;
Mandelbrot* mandelbrot;

double l = -2.25; double r = 0.75; double t = 1.5; double b = -1.5; // default

//double l = -0.17160144327474860515; double r = -0.17142732819507847108; double t = 0.8350831025283045328; double b = 0.83490898744863506487; // default
//double l = -0.603531; double r = -0.488737; double t = 0.65485; double b = 0.568753;
//double l = -0.394687; double r = -0.378788; double t = 0.629904; double b = 0.61798;
//double centre_x = -0.17151439017639816265; double centre_y = 0.83499604942995442336;
//double l = -0.17151439025976980557; double r = -0.17151439009094221477; double t = 0.8349960495133261773; double b = 0.83499604934449844773;
// 
//int max_iterations = 1310;
//double centre_x = -0.17151439017639816265; double centre_y = 0.83499604942995442336;

int max_iterations = 128;

double view_width = std::abs(r - l);
double view_height = std::abs(t - b);

double zoom_iter_add = max_iterations;

void runFarm() {

	Farm farm;
	const double num_segments = 200.0;
	const double slice = (double)height / num_segments;

	for (int i = 0; i < num_segments; i++) {
		farm.add_task(new MandelbrotTask(mandelbrot->getImage(), l, r, t, b, i * slice, i * slice + slice, max_iterations));
	}

	farm.run();
	mandelbrot->update();

	std::cout << std::setprecision(20) << "double l = " << l << "; double r = " << r << "; double t = " << t << "; double b = " << b << ";" << std::endl << "int max_iterations = " << max_iterations << ";" << std::endl;;
}

void updateViewSize() {
	// update view size variables since these will change everytime we move/zoom
	view_width = std::abs(r - l);
	view_height = std::abs(t - b);
}

void lerpToPos(double x, double y, double m_t, double z_t){

	// x, y is the center of the new view rectangle, m_t is the movement lerp "time" (0-1), z_t is the zoom lerp "time" (0-1), z_t can be negative to zoom out.

	std::cout <<  "\n\ndouble centre_x = " << x << "; double centre_y = " << y << ";" << std::endl;

	updateViewSize();

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

	zoom_iter_add += (double)100 * z_t;
	std::cout << zoom_iter_add << std::endl;
	max_iterations = (int)zoom_iter_add; // increase detail (max_iterations) as we zoom in, and decrease as we zoom out
	if (max_iterations <= 0) {
		max_iterations = 10;
	}
}

void handleMouseInput() {
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		lerpToPos(sf::Mouse::getPosition(*window).x / (double)width * view_width + l, sf::Mouse::getPosition(*window).y / (double)height * -view_height + t, 1.0, 0.3);
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		lerpToPos(sf::Mouse::getPosition(*window).x / (double)width * view_width + l, sf::Mouse::getPosition(*window).y / (double)height * -view_height + t, 1.0, -0.3);
	}
}

int main()
{
	window = new sf::RenderWindow(sf::VideoMode(width, height), "Mandelbrot");
	mandelbrot = new Mandelbrot(width, height);
	
	runFarm();

	//sf::CircleShape xhair;
	//xhair.setFillColor(sf::Color::Green);
	//xhair.setRadius(1.0f);
	//xhair.setPosition(((float)width / 2.f) - xhair.getGlobalBounds().width / 2.f, ((float)height / 2.f) - xhair.getGlobalBounds().height / 2.f);
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

		//lerpToPos(-0.101096, 0.956302, 0.5, 0.01);
		//lerpToPos(-0.17151439017639816265, 0.83499604942995442336, 0.5, 0.005);

		window->clear();
		window->draw(*mandelbrot);
		//window->draw(xhair);
		window->display();
	}

	return 0;
}