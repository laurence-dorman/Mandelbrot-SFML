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

int scale_factor = 100;

int max_iterations = 200;

double wh_scale = (double)width / (double)height;

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

void updateViewSize() 
{
	view_width = std::abs(r - l);
	view_height = std::abs(t - b);
}

int main()
{
	window = new sf::RenderWindow(sf::VideoMode(width, height), "Mandelbrot");
	mandelbrot = new Mandelbrot(width, height);

	runFarm();

	sf::CircleShape xhair;
	xhair.setFillColor(sf::Color::Red);
	xhair.setRadius(0.5f);
	xhair.setPosition(width / 2, height / 2);

	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();

			if (event.type == sf::Event::KeyPressed)
			{
				double zoom_factor_h = 0.1;
				double zoom_factor_w = zoom_factor_h * wh_scale;

				double pan_y = 0.1;
				double pan_x = 0.1 * wh_scale;

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) 
				{
					l = std::lerp(l, r, 0.1);
					r = std::lerp(r, l, 0.1);

					t = std::lerp(t, b, 0.1);
					b = std::lerp(b, t, 0.1);

					scale_factor+=10;

					updateViewSize();
					runFarm();
					std::cout << std::endl;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				{
					l -= zoom_factor_w;
					r += zoom_factor_w;

					t += zoom_factor_h;
					b -= zoom_factor_h;

					if (scale_factor > 10) {
						scale_factor -= 10;
					}

					runFarm();
				}
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				//handle_mouse_input();
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) 
				{
					updateViewSize();

					double click_pos_x = sf::Mouse::getPosition(*window).x / (double)width * view_width + l;
					double click_pos_y = sf::Mouse::getPosition(*window).y / (double)height * -view_height + t;

					l = std::lerp(l, click_pos_x - view_width/2, 1.0);
					r = std::lerp(r, click_pos_x + view_width/2, 1.0);

					t = std::lerp(t , click_pos_y + view_height / 2, 1.0);
					b = std::lerp(b , click_pos_y - view_height / 2, 1.0);

					l = std::lerp(l, r, 0.1);
					r = std::lerp(r, l, 0.1);

					t = std::lerp(t, b, 0.1);
					b = std::lerp(b, t, 0.1);

					max_iterations += 20.0;

					runFarm();
				}
			}
		}

		window->clear();
		window->draw(*mandelbrot);
		window->draw(xhair);
		window->display();
	}

	return 0;
}