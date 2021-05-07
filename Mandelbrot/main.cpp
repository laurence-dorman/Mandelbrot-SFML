#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <chrono>

#include "farm.h"
#include "task.h"
#include "mandelbrot_task.h"
#include "multibrot_task.h"
#include "fractal.h"
#include "colour_manager.h"

using std::chrono::duration_cast;
using std::chrono::milliseconds;

typedef std::chrono::high_resolution_clock the_clock;

sf::RenderWindow* window;
Fractal* fractal;
ColourManager* colourManager;

int mode = 1; // mandelbrot is 1, multibrot is 2

// initial values for view square
double l = -2.25; double r = 0.75; double t = 1.5; double b = -1.5; // default

// initial max_iterations value and value for zooming
int max_iterations = 128;
double zoom_iter_add = max_iterations;

// setup view vars to be the difference between r,l and t,b for width and height respectively
double view_width = std::abs(r - l);
double view_height = std::abs(t - b);

// set window size to be a square 85% of window height
unsigned int screen_height = sf::VideoMode::getDesktopMode().height*0.85;
unsigned int width = screen_height;
unsigned int height = screen_height;

// vars for animations
bool lerping = false;
int frames_done = 0;
int num_frames = 20;
double pos_x = 0, pos_y = 0;
double zoom_time = 0.01;
the_clock::time_point start_animation;

std::string file_name = "output";

void runFarm() {

	Farm farm;

	const double num_segments = height;
	const double slice = (double)height/num_segments;

	for (int i = 0; i < num_segments; i++) {
		switch (mode) 
		{
			case 1:
				farm.add_task(new MandelbrotTask(fractal->getImage(), l, r, t, b, i * slice, i * slice + slice, max_iterations, colourManager->current_scheme, width, height, *colourManager->getSchemesVec()));
			break;

			case 2:
				farm.add_task(new MultibrotTask(fractal->getImage(), l, r, t, b, i * slice, i * slice + slice, max_iterations, colourManager->current_scheme, width, height, *colourManager->getSchemesVec()));
			break;
		}
	}

	// Start timing
	the_clock::time_point start = the_clock::now();

	farm.run();
	fractal->update(lerping, file_name.c_str());

	// Stop timing
	the_clock::time_point end = the_clock::now();

	// Compute the difference between the two times in milliseconds
	auto time_taken = duration_cast<milliseconds>(end - start).count();

	std::cout << std::setprecision(20) << 
		"Mode: " << mode <<
		"\nLeft X: " << l << 
		"\nBottom Y: " << b << 
		"\nWidth: " << view_width << 
		"\nHeight: " << view_height << 
		"\nIterations: " << max_iterations << 
		"\nTheme: " << colourManager->current_scheme <<
		"\nTime taken: " << time_taken <<  "ms." << 
		std::endl << std::endl;
}

void updateViewSize() {
	// update view size variables since these will change everytime we move/zoom
	view_width = std::abs(r - l);
	view_height = std::abs(t - b);
}

void reset() { 
	// reset to initial square
	l = -2.25; r = 0.75; t = 1.5; b = -1.5;
	max_iterations = 128;
	zoom_iter_add = max_iterations;

	updateViewSize();
	runFarm();
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

void runAnimation(int frames, double m_t, double z_t) {
	// creates frames for an animation and saves to output folder
	if (frames_done < frames) {
		if (frames_done == 0) {

			pos_x = sf::Mouse::getPosition(*window).x / (double)width * view_width + l;
			pos_y = sf::Mouse::getPosition(*window).y / (double)height * -view_height + t;

			int u_i;
			std::cout << "\nWould you like to use mouse location? (0 = no, 1 = yes)\n>> ";
			std::cin >> u_i;

			if (u_i == 0) {
				std::cout << "Enter the X position of your desired location\n>> ";
				std::cin >> pos_x;
				std::cout << "Enter the Y position of your desired location\n>> ";
				std::cin >> pos_y;
			}

			std::cout << "\nEnter the amount of frames for the animation\n>> ";
			std::cin >> num_frames;

			std::cout << "\nEnter how much you want to zoom in per frame (recommended 0-100)\n>> ";

			double u_input;
			std::cin >> u_input;
			zoom_time = u_input / 150; // will be used in lerp function

			std::cout << "\nEnter the filename\n>> ";
			std::cin >> file_name;

			start_animation = the_clock::now(); // start timer
	
			reset(); // go to start
		}
		lerpToPos(pos_x, pos_y, m_t, z_t); // lerp to pos
		frames_done++; // incrase frames done
	}
	else {
		// Stop timing
		the_clock::time_point end = the_clock::now();

		// Compute the difference between the two times in milliseconds
		auto time_taken = duration_cast<milliseconds>(end - start_animation).count();

		std::cout << "Total time taken for animation: " << time_taken << "ms.\n\n";

		lerping = false;
		frames_done = 0;
		reset();
	}
}

void handleMouseInput() 
{	// convert click location on window to coordinates inside the mandelbrot square and then lerp to it
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		lerpToPos(sf::Mouse::getPosition(*window).x / (double)width * view_width + l, sf::Mouse::getPosition(*window).y / (double)height * -view_height + t, 1.0, 0.3); 
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		lerpToPos(sf::Mouse::getPosition(*window).x / (double)width * view_width + l, sf::Mouse::getPosition(*window).y / (double)height * -view_height + t, 1.0, -0.3);
	}
}

void handleScrollInput(float delta) 
{
	max_iterations += delta*10;
	zoom_iter_add = max_iterations;
	runFarm();
}

void handleKeyboardInput() 
{
	bool redraw = colourManager->handleInput();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
		mode = 1;
		redraw = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
		mode = 2;
		redraw = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
		reset();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
		if (lerping == false) {
			lerping = true;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		window->close();
	}

	if (redraw) runFarm();
}

int main()
{
	window = new sf::RenderWindow(sf::VideoMode(width, height), "Mandelbrot " + std::to_string(width) + "x" + std::to_string(height), sf::Style::Titlebar);

	fractal = new Fractal(width, height);
	colourManager = new ColourManager();

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
				handleScrollInput(event.mouseWheelScroll.delta);
			}
		}

		if (lerping) {
			runAnimation(num_frames, 1.0, zoom_time);
		}

		window->clear();
		window->draw(*fractal);
		window->display();
	}

	return 0;
}