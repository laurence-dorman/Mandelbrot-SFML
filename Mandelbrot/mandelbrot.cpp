#include "mandelbrot.h"
#include <iostream>

Mandelbrot::Mandelbrot(unsigned int width, unsigned int height)
{
	texture.create(width, height);
	image.create(width, height, sf::Color(0, 0, 0));
	setTexture(texture);
}

void Mandelbrot::update(bool save, std::string fn)
{
	texture.update(image);

	if (save) {
		std::cout << "Saving image to output/" + fn + std::to_string(i) + ".png" << std::endl;
		image.saveToFile("output/" + fn + std::to_string(i) + ".png");
		i++;
	}
}
