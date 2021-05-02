#pragma once

#include <SFML/Graphics/Color.hpp>

// Mandelbrot colouring method from here https://github.com/sevity/mandelbrot/blob/master/nothing.cpp and discussed here https://stackoverflow.com/questions/16500656/which-color-gradient-is-used-to-color-mandelbrot-in-wikipedia

inline sf::Color linearInterpolation(sf::Color& v, const sf::Color& u, double a) {
	auto const b = 1 - a;
	return sf::Color(b * v.r + a * u.r, b * v.g + a * u.g, b * v.b + a * u.b);
}

inline sf::Uint8 generateColour()
{
	return sf::Uint8(rand() + 255);
}
