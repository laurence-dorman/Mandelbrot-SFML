#include <SFML/Graphics.hpp>
#include <vector>
#include <thread>
#include <functional>
#include <complex>
#include <mutex>

using std::complex;
using std::thread;

const unsigned int width = 1280;
const unsigned int height = 960;

const unsigned int max_iterations = 100;

uint32_t image[height][width];

sf::Image image_;
sf::Sprite mandelbrot_;

sf::RenderWindow* window;

std::mutex image_mutex;

sf::Color getColour(int i)
{
	int colourVal = (255.f / (float)max_iterations) * i;
	int blueAdd = (std::abs(colourVal - 255) < 50) ? std::abs(colourVal - 255) : 50; // adds 50 to blue value but never exceeds 255 (probably bad way of doing it)

	return sf::Color(colourVal, colourVal, colourVal + blueAdd);
}

void compute_mandelbrot(float left, float right, float top, float bottom, double y_start, double y_end)
{
	for (int y = y_start; y < y_end; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			// Work out the point in the complex plane that
			// corresponds to this pixel in the output image.
			complex<double> c(left + (x * (right - left) / width),
				top + (y * (bottom - top) / height));

			// Start off z at (0, 0).
			complex<double> z(0.0, 0.0);

			// Iterate z = z^2 + c until z moves more than 2 units
			// away from (0, 0), or we've iterated too many times.
			int iterations = 0;
			while (abs(z) < 2.0 && iterations < max_iterations)
			{
				z = (z * z) + c;

				++iterations;
			}
			
			if (iterations == max_iterations)
			{
				// z didn't escape from the circle.
				// This point is in the Mandelbrot set.
				image_.setPixel(x, y, sf::Color(0, 0, 0)); // black
			}
			else
			{
				// z escaped within less than MAX_ITERATIONS
				// iterations. This point isn't in the set.
				image_.setPixel(x, y, getColour(iterations)); // get colour based on iterations
			}
			
		}
	}
}

void do_mandelbrot(float left, float right, float top, float bottom)
{
	std::vector<thread*> threads_vec;

	const int num_threads = 16;
	const double slice = height / num_threads;

	for (int i = 0; i < num_threads; i++) {
		threads_vec.push_back(new thread(compute_mandelbrot, left, right, top, bottom,  i * slice, i * slice + slice));
	}

	for (std::thread* THREAD : threads_vec) {
		THREAD->join();
	}
}

int main()
{
	window = new sf::RenderWindow(sf::VideoMode(width, height), "Mandelbrot");

	sf::Texture texture_;
	texture_.create(width, height);
	image_.create(width, height, sf::Color(0, 0, 0));

	do_mandelbrot(-2.0, 1.0, 1.125, -1.125);

	texture_.update(image_);
	mandelbrot_.setTexture(texture_);

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
		window->draw(mandelbrot_);
		window->display();
	}

	return 0;
}