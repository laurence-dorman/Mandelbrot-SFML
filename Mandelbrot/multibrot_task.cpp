#include "multibrot_task.h"

MultibrotTask::MultibrotTask(sf::Image* image, double left, double right, double top, double bottom, double y_start, double y_end, int iterations, int scheme, unsigned int width, unsigned int height, std::vector<std::vector<sf::Color>>& c_schemes) :
	image_(image),
	left_(left),
	right_(right),
	top_(top),
	bottom_(bottom),
	y_start_(y_start),
	y_end_(y_end),
	iterations_(iterations),
	current_scheme(scheme),
	width_(width),
	height_(height),
	colour_schemes_ptr(&c_schemes)
{
}

void MultibrotTask::run()
{
	for (double y = y_start_; y < y_end_; ++y)
	{
		for (double x = 0; x < width_; ++x)
		{
			// Work out the point in the complex plane that
			// corresponds to this pixel in the output image.
			complex<double> c(left_ + (x * (right_ - left_) / width_),
				top_ + (y * (bottom_ - top_) / height_));


			// Start off z at (-1, 0).
			complex<double> z(-1.0, 0.0);

			// Iterate z = z^-6 + c until z moves more than 2 units
			// away from (0, 0), or we've iterated too many times.
			int iterations = 0;
			while (abs(z) < 2.0 && iterations < iterations_)
			{
				z = 1.0/(z*z*z*z*z*z) + c;
				++iterations;
			}

			if (iterations == iterations_)
			{
				// z didn't escape from the circle.
				// This point is in the Multibrot set.

				image_->setPixel(x, y, sf::Color::Black); // black

			}
			else
			{
				// z escaped within less than MAX_ITERATIONS
				// iterations. This point isn't in the set.

				if (current_scheme != -1) {
					image_->setPixel(x, y, getColour(iterations)); // get colour based on iterations
				}
				else {
					image_->setPixel(x, y, sf::Color::White); // get colour based on iterations
				}
			}
		}
	}
}


sf::Color MultibrotTask::getColour(int i)
{
	auto colour = (*colour_schemes_ptr)[current_scheme];

	auto max_color = colour.size() - 1;
	if (i == iterations_) i = 0;

	double mu = 1.0 * i / iterations_;
	mu *= max_color;
	auto i_mu = static_cast<size_t>(mu);

	return linearInterpolation(colour[i_mu], colour[std::min(i_mu + 1, max_color)], mu - i_mu);
}