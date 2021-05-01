#include "mandelbrot.h"
#include <iostream>
#include<direct.h>

Mandelbrot::Mandelbrot(unsigned int width, unsigned int height)
{
	texture.create(width, height);
	image.create(width, height, sf::Color(0, 0, 0));
	setTexture(texture);

	path_c = folder.c_str();
}

void Mandelbrot::update(bool save, std::string fn)
{
	texture.update(image); // update texture

	if (save) {
		std::cout << "Saving image to output/" + fn + std::to_string(i) + ".png" << std::endl;

		if (!folder_created) { // create output folder
			success = _mkdir(path_c);
			folder_created = true;
		}

		if (!image.saveToFile("output/" + fn + std::to_string(i) + ".png")){ // if we cant create files inside new folder for some reason, just save them in the root directory
			std::cout << "\nCould not find output folder! Writing to current directory instead.\n";
			std::cout << "Saving image to " + fn + std::to_string(i) + ".png" << std::endl;
			image.saveToFile(fn + std::to_string(i) + ".png");
		}
		i++; // increment i (used in filename)
	}
}