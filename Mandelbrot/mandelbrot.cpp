#include "mandelbrot.h"

const unsigned int max_iterations = 10;

Mandelbrot::Mandelbrot(int width, int height):
	width_(width),
	height_(height)
{
	texture_.create(width_, height_);
	image_.create(width, height, sf::Color(0, 0, 0));

	this->setTexture(texture_);
}

// The parameters specify the region on the complex plane to plot.
void Mandelbrot::compute_mandelbrot(sf::Rect<long double> view, double y_start, double y_end)
{
	for (int y = y_start; y < y_end; ++y)
	{
		for (int x = 0; x < width_; ++x)
		{
			// Work out the point in the complex plane that
			// corresponds to this pixel in the output image.
			complex<long double> c(view.left + (x * (view.left + view.width - view.left) / width_),
				view.top + (y * (view.top + view.height - view.top) / height_));

			// Start off z at (0, 0).
			complex<long double> z(0.0, 0.0);

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
	texture_.update(image_);
}

void Mandelbrot::do_mandelbrot(sf::Rect<long double> view)
{
	std::vector<thread*> threads_vec;

	const int num_threads = 16;
	const double slice = height_ / num_threads;

	for (int i = 0; i < 16; i++) {
		sf::Rect<long double> view_ = view;

		threads_vec.push_back(new thread(std::bind(&Mandelbrot::compute_mandelbrot, this, view_, i * slice, i * slice + slice), view_, i * slice, i * slice + slice));
	}

	for (std::thread* THREAD : threads_vec) {
		THREAD->join();
	}
}

sf::Color Mandelbrot::getColour(int i)
{
	int colourVal = (255.f / (float)max_iterations) * i;
	int blueAdd = (std::abs(colourVal - 255) < 50) ? std::abs(colourVal - 255) : 50; // adds 50 to blue value but never exceeds 255 (probably bad way of doing it)

	return sf::Color(colourVal, colourVal, colourVal + blueAdd);
}
