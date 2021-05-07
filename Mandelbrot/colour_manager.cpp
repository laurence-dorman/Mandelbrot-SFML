#include "colour_manager.h"

#include <iomanip>
#include <iostream>
#include <SFML/Window/Keyboard.hpp>

#include "utils.h"

ColourManager::ColourManager()
{
	srand(time(NULL));

	std::vector<sf::Color> default_scheme
	{
		{0, 7, 100},		// dark blue
		{32, 107, 203},		// light blue
		{237, 255, 255},	// white tinted green
		{255, 170, 0},		// orange
		{0, 2, 0},			// black
	};

	std::vector<sf::Color> green_scheme
	{
		{0, 100, 7},		// dark green
		{32, 203, 107},		// light green
		{237, 255, 155},	// white tinted green
		{255, 170, 0},		// orange
		{0, 0, 0},			// black
	};

	std::vector<sf::Color> red_scheme
	{
		{100, 0, 7},		// dark red
		{203, 32, 107},		// light red
		{255, 155, 155},	// white tinted red
		{255, 0, 255},		// pink
		{0, 0, 0},			// black
	};

	std::vector<sf::Color> purple_scheme
	{
		{128, 0, 128},		// dark purple
		{255,255,25},		// laser lemon
		{57, 255, 20},		// neon green
		{128, 0, 128},		// dark purple
		{0, 2, 0},			// black
	};

	colour_schemes.push_back(default_scheme);	// 0
	colour_schemes.push_back(green_scheme);		// 1
	colour_schemes.push_back(red_scheme);		// 2
	colour_schemes.push_back(purple_scheme);	// 3
	addRandomScheme(0);							// 4

	num_schemes = colour_schemes.size();
}

ColourManager::~ColourManager()
{
}

void ColourManager::addRandomScheme(int rows)
{
	int num_colours = rows;
	if (rows == 0) {
		num_colours = rand() % 30 + 3; // how many rows
	}

	std::vector<sf::Color> new_scheme;

	for (int i = 0; i < num_colours; i++) {
		new_scheme.push_back({ generateColour(), generateColour(), generateColour() }); // rgb
	}

	colour_schemes.push_back(new_scheme);

	num_schemes++;
}

bool ColourManager::handleInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		current_scheme++;
		if (current_scheme > num_schemes - 1) current_scheme = -1;
		return true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		current_scheme--;
		if (current_scheme < -1) current_scheme = num_schemes - 1;
		return true;
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
		int u_in_rows = 0;
		std::cout << "Enter how many colours you want in the theme, or enter 0 for a random amount.\n>> ";
		std::cin >> u_in_rows;
		addRandomScheme(u_in_rows);
		current_scheme = colour_schemes.size() - 1;
		return true;
	}
	return false;
}

std::vector<std::vector<sf::Color>> *ColourManager::getSchemesVec()
{
	return &colour_schemes;
}
