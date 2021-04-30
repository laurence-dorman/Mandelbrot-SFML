#include "mandelbrot_task.h"

MandelbrotTask::MandelbrotTask(sf::Image* image, double left, double right, double top, double bottom, double y_start, double y_end, int iterations, int scheme) :
	image_(image),
	left_(left),
	right_(right),
	top_(top),
	bottom_(bottom),
	y_start_(y_start),
	y_end_(y_end),
	iterations_(iterations),
	current_scheme(scheme)
{
	std::vector<sf::Color> default_scheme
	{
		{0, 7, 100},		// dark blue
		{32, 107, 203},		// light blue
		{237, 255, 255},	// white tinted green
		{255, 170, 0},		// orange
		{0, 2, 0},			// black
	};

	std::vector<sf::Color> green_scheme
	{
		{0, 100, 7},		// dark green
		{32, 203, 107},		// light green
		{237, 255, 155},	// white tinted green
		{255, 170, 0},		// orange
		{0, 0, 0},			// black
	};

	std::vector<sf::Color> red_scheme
	{
		{100, 0, 7},		// dark red
		{203, 32, 107},		// light red
		{255, 155, 155},	// white tinted red
		{255, 0, 255},		// pink
		{0, 0, 0},			// black
	};

	colour_schemes.push_back(default_scheme);
	colour_schemes.push_back(green_scheme);
	colour_schemes.push_back(red_scheme);

}


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
			while (abs(z) < 2.0 && iterations < iterations_)
			{
				z = (z * z) + c;

				++iterations;
			}

			if (iterations == iterations_)
			{
				// z didn't escape from the circle.
				// This point is in the Mandelbrot set.

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

// Mandelbrot colouring method from here https://github.com/sevity/mandelbrot/blob/master/nothing.cpp and discussed here https://stackoverflow.com/questions/16500656/which-color-gradient-is-used-to-color-mandelbrot-in-wikipedia

sf::Color linearInterpolation(sf::Color& v, const sf::Color& u, double a) {
	auto const b = 1 - a;
	return sf::Color(b * v.r + a * u.r, b * v.g + a * u.g, b * v.b + a * u.b);
}

sf::Color MandelbrotTask::getColour(int i)
{
	auto colour = colour_schemes[current_scheme];
	
	auto max_color = colour.size() - 1;
	if (i == iterations_) i = 0;
	
	double mu = 1.0 * i / iterations_;
	mu*= max_color;
	auto i_mu = static_cast<size_t>(mu);
	
	return linearInterpolation(colour[i_mu], colour[std::min(i_mu + 1, max_color)], mu - i_mu);
}