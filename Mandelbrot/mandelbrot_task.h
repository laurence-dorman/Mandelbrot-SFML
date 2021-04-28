#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Color.hpp>
#include <complex>

#include "task.h"

using std::complex;

const unsigned int width = 1280;
const unsigned int height = 960;

const unsigned int max_iterations = 200;

/** Task that computes a portion of a mandelbrot */
class MandelbrotTask : public Task
{
public:
	MandelbrotTask(sf::Image* image, double left, double right, double top, double bottom, double y_start, double y_end) : 
		image_(image),
		left_(left),
		right_(right),
		top_(top),
		bottom_(bottom),
		y_start_(y_start),
		y_end_(y_end)
	{
	}

	void run();

private:
	double left_; double right_; double top_; double bottom_; double y_start_; double y_end_;

	sf::Image* image_;

	sf::Color getColour(int i);
};