#pragma once

#include <SFML/Graphics/Color.hpp>
#include <vector>


class ColourManager
{
public:
	ColourManager();
	~ColourManager();

	void addRandomScheme(int rows);
	bool handleInput();

	std::vector<std::vector<sf::Color>> *getSchemesVec();

	int current_scheme;
	int num_schemes;

private:

	std::vector<std::vector<sf::Color>> colour_schemes;
	
};

