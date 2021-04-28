#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Color.hpp>
#include <complex>

#include "task.h"

using std::complex;

const unsigned int width = 1280;
const unsigned int height = 960;

const unsigned int max_iterations = 100;

/** Task that computes a portion of a mandelbrot */
class MandelbrotTask : public Task
{
public:
	MandelbrotTask(sf::Image* image, float left, float right, float top, float bottom, double y_start, double y_end) : 
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
	float left_; float right_; float top_; float bottom_; double y_start_; double y_end_;

	sf::Image* image_;

	sf::Color getColour(int i);
};