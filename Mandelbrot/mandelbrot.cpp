#include "mandelbrot.h"

Mandelbrot::Mandelbrot(unsigned int width, unsigned int height)
{
	texture.create(width, height);
	image.create(width, height, sf::Color(0, 0, 0));
	setTexture(texture);
}

void Mandelbrot::update()
{
	texture.update(image);
}
