#include "mandelbrot_task.h"

void MandelbrotTask::run() 
{
	for (double y = y_start_; y < y_end_; ++y)
	{
		for (double x = 0; x < width; ++x)
		{
			// Work out the point in the complex plane that
			// corresponds to this pixel in the output image.
			complex<double> c(left_ + (x * (right_ - left_) / width),
				top_ + (y * (bottom_ - top_) / height));

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
				image_->setPixel(x, y, sf::Color(0, 0, 0)); // black
			}
			else
			{
				// z escaped within less than MAX_ITERATIONS
				// iterations. This point isn't in the set.
				image_->setPixel(x, y, getColour(iterations)); // get colour based on iterations
			}

		}
	}
}

sf::Color MandelbrotTask::getColour(int i)
{
	int colourVal = (255.f / (float)max_iterations) * i;
	int blueAdd = (std::abs(colourVal - 255) < 50) ? std::abs(colourVal - 255) : 50; // adds 50 to blue value but never exceeds 255 (probably bad way of doing it)

	return sf::Color(colourVal, colourVal, colourVal + blueAdd);
}