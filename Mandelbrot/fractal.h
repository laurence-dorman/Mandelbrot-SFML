#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class Fractal : public sf::Sprite
{
public:

	Fractal(unsigned int width, unsigned int height);

	void update(bool save, std::string fn);

	sf::Image* getImage() { return &image; };

private:
	sf::Texture texture;
	sf::Image image;

	int i;

};

