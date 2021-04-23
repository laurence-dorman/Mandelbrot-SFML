#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <thread>
#include <complex>
#include <functional>

using std::thread;
using std::complex;

class Mandelbrot : public sf::Sprite
{
public:
	Mandelbrot(int width, int height);

	void compute_mandelbrot(sf::Rect<long double> view, double y_start, double y_end);

	void do_mandelbrot(sf::Rect<long double> view);

	

private:
	
	int width_, height_;

	long double zoom_x = 0.0, zoom_y = 0.0, pan_x = 0.0, pan_y = 0.0;

	sf::Image image_;
	sf::Texture texture_;

	sf::Color getColour(int i);

};

