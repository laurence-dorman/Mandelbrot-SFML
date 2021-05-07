#include "fractal.h"
#include <iostream>
#include <direct.h>

Fractal::Fractal(unsigned int width, unsigned int height) :
	i(0)
{
	texture.create(width, height);
	image.create(width, height, sf::Color(0, 0, 0));
	setTexture(texture);

	if (!_mkdir("output")) {
		std::cout << "Created output directory.\n\n";
	}
}

void Fractal::update(bool save, const char* fn)
{
	texture.update(image); // update texture

	if (save) {
		std::string s_fn(fn);
		std::cout << "Saving image to output/" + s_fn + std::to_string(i) + ".png" << std::endl;

		if (!image.saveToFile("output/" + s_fn + std::to_string(i) + ".png")){ // if we cant create files inside new folder for some reason, just save them in the root directory
			std::cout << "\nCould not find output folder! Writing to current directory instead.\n";
			std::cout << "Saving image to " + s_fn + std::to_string(i) + ".png" << std::endl;
			image.saveToFile(s_fn + std::to_string(i) + ".png");
		}
		i++; // increment i (used in filename)
	}
}