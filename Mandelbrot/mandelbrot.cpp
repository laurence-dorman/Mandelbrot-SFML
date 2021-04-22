#include "mandelbrot.h"

const unsigned int max_iterations = 100;

Mandelbrot::Mandelbrot(int width, int height):
	width_(width),
	height_(height)
{
	texture_.create(width_, height_);
	image_.create(width, height, sf::Color(0, 0, 0));

	this->setTexture(texture_);
}

// The parameters specify the region on the complex plane to plot.
void Mandelbrot::compute_mandelbrot(double left, double right, double top, double bottom, double y_start, double y_end)
{
	for (int y = y_start; y < y_end; ++y)
	{
		for (int x = 0; x < width_; ++x)
		{
			// Work out the point in the complex plane that
			// corresponds to this pixel in the output image.
			complex<double> c(left + (x * (right - left) / width_),
				top + (y * (bottom - top) / height_));

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
	texture_.update(image_);
}

sf::Color Mandelbrot::getColour(int i)
{
	int colourVal = (255.f / (float)max_iterations) * i;
	int blueAdd = (std::abs(colourVal - 255) < 50) ? std::abs(colourVal - 255) : 50; // adds 50 to blue value but never exceeds 255 (probably bad way of doing it)

	return sf::Color(colourVal, colourVal, colourVal + blueAdd);
}
