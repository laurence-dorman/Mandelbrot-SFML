#pragma once

#include <SFML/Graphics.hpp>
#include <complex>

using std::complex;

class Mandelbrot : public sf::Sprite
{
public:
	Mandelbrot(int width, int height);

	void compute_mandelbrot(double left, double right, double top, double bottom, double y_start, double y_end);

private:

	int width_, height_;

	sf::Image image_;
	sf::Texture texture_;

	sf::Color getColour(int i);

};

