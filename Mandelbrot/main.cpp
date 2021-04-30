#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <chrono>

#include "farm.h"
#include "task.h"
#include "mandelbrot_task.h"
#include "mandelbrot.h"

using std::chrono::duration_cast;
using std::chrono::milliseconds;

typedef std::chrono::high_resolution_clock the_clock;

sf::RenderWindow* window;
Mandelbrot* mandelbrot;

double l = -2.25; double r = 0.75; double t = 1.5; double b = -1.5; // default

int max_iterations = 128;

double view_width = std::abs(r - l);
double view_height = std::abs(t - b);

unsigned int screen_height = sf::VideoMode::getDesktopMode().height*0.85;

unsigned int width = screen_height;
unsigned int height = screen_height;

double zoom_iter_add = max_iterations;

int num_schemes = 3;
int scheme = 0;

void runFarm() {

	Farm farm;

	const double num_segments = screen_height;
	const double slice = 1.0;

	for (int i = 0; i < num_segments; i++) {
		farm.add_task(new MandelbrotTask(mandelbrot->getImage(), l, r, t, b, i * slice, i * slice + slice, max_iterations, scheme, width, height));
	}

	// Start timing
	the_clock::time_point start = the_clock::now();

	farm.run();
	mandelbrot->update();

	// Stop timing
	the_clock::time_point end = the_clock::now();

	// Compute the difference between the two times in milliseconds
	auto time_taken = duration_cast<milliseconds>(end - start).count();

	//std::cout << std::setprecision(20) << "double l = " << l << "; double r = " << r << "; double t = " << t << "; double b = " << b << ";" << std::endl << "int max_iterations = " << max_iterations << ";" << std::endl;
	std::cout << std::setprecision(20) << 
		"Left X: " << l << 
		"\nBottom Y: " << b << 
		"\nWidth: " << view_width << 
		"\nHeight: " << view_height << 
		"\nIterations: " << max_iterations << 
		"\nTheme: " << scheme << 
		"\nTime taken: " << time_taken <<  "ms." << 
		std::endl << std::endl;
}

void updateViewSize() {
	// update view size variables since these will change everytime we move/zoom
	view_width = std::abs(r - l);
	view_height = std::abs(t - b);
}

void lerpToPos(double x, double y, double m_t, double z_t){

	// x, y is the center of the new view rectangle, m_t is the movement lerp "time" (0-1), z_t is the zoom lerp "time" (0-1), z_t can be negative to zoom out.

	//std::cout <<  "\n\ndouble centre_x = " << x << "; double centre_y = " << y << ";" << std::endl;
	std::cout << "Lerping to pos: " << x << ", " << y << std::endl;

	updateViewSize();

	// zoom in (lerp the edges toward eachother based on z_t)
	l = std::lerp(l, r, z_t);
	r = std::lerp(r, l, z_t);

	t = std::lerp(t, b, z_t);
	b = std::lerp(b, t, z_t);

	updateViewSize();

	// move to new view rectangle (lerp the edges toward their new edge centered around x, y, based on m_t)

	l = std::lerp(l, x - view_width / 2, m_t);
	r = std::lerp(r, x + view_width / 2, m_t);

	t = std::lerp(t, y + view_height / 2, m_t);
	b = std::lerp(b, y - view_height / 2, m_t);

	updateViewSize();
	runFarm();

	zoom_iter_add += (double)100 * z_t;
	max_iterations = (int)zoom_iter_add; // increase detail (max_iterations) as we zoom in, and decrease as we zoom out
	if (max_iterations <= 0) {
		max_iterations = 10;
	}
}

void handleMouseInput() 
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		lerpToPos(sf::Mouse::getPosition(*window).x / (double)width * view_width + l, sf::Mouse::getPosition(*window).y / (double)height * -view_height + t, 1.0, 0.3);
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		lerpToPos(sf::Mouse::getPosition(*window).x / (double)width * view_width + l, sf::Mouse::getPosition(*window).y / (double)height * -view_height + t, 1.0, -0.3);
	}
}

void handleScrollInput(sf::Event* event) 
{
	max_iterations += event->mouseWheelScroll.delta*10;
	zoom_iter_add = max_iterations;
	runFarm();
}

void handleKeyboardInput() 
{
	bool redraw = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		scheme++;
		if (scheme > num_schemes - 1) scheme = -1;
		redraw = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		scheme--;
		if (scheme < -1) scheme = num_schemes - 1;
		redraw = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		window->close();
	}

	if (redraw) runFarm();
}

int main()
{
	window = new sf::RenderWindow(sf::VideoMode(screen_height, screen_height), "Mandelbrot", sf::Style::Titlebar);
	mandelbrot = new Mandelbrot(width, height);
	
	runFarm();

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
				handleKeyboardInput();
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				handleMouseInput();
			}
			if (event.type == sf::Event::MouseWheelScrolled) 
			{
				handleScrollInput(&event);
			}
		}

		//lerpToPos(-0.17151439017639816265, 0.83499604942995442336, 0.5, 0.005);
		//lerpToPos(-1.2102340002812257413, 0.17016643387500379747, 0.3, 0.02);

		window->clear();
		window->draw(*mandelbrot);
		window->display();
	}

	return 0;
}