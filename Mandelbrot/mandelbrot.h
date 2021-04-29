#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class Mandelbrot : public sf::Sprite
{
public:

	Mandelbrot(unsigned int width, unsigned int height);

	void update();

	sf::Image* getImage() { return &image; };

private:
	sf::Texture texture;
	sf::Image image;

	int i = 0;

};

