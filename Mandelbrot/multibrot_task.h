#pragma once
#include <SFML/Graphics/Image.hpp>
#include <complex>

#include "task.h"
#include "utils.h"

using std::complex;

/** Task that computes a portion of a Multibrot set
https://en.wikipedia.org/wiki/Mandelbrot_set
*/
class MultibrotTask : public Task
{
public:
	MultibrotTask(sf::Image* image, double left, double right, double top, double bottom, double y_start, double y_end, int iterations, int scheme, unsigned int width, unsigned int height, std::vector<std::vector<sf::Color>>& c_schemes);

	void run();

private:

	double left_; double right_; double top_; double bottom_; double y_start_; double y_end_;
	int iterations_;

	unsigned int width_, height_;

	sf::Image* image_;

	sf::Color getColour(int i);

	// vector that holds vectors of sf::Colors, so we can switch between different themes
	std::vector<std::vector<sf::Color>> *colour_schemes_ptr;

	int current_scheme = 0;

};

