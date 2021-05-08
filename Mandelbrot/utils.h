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

inline sf::Color getColour(int i, std::vector<std::vector<sf::Color>> *col_scheme_ptr, int current_scheme, int max_i)
{
	auto colour = (*col_scheme_ptr)[current_scheme];

	auto max_color = colour.size() - 1;
	if (i == max_i) i = 0;

	double mu = 1.0 * i / max_i;
	mu *= max_color;
	auto i_mu = static_cast<size_t>(mu);

	return linearInterpolation(colour[i_mu], colour[std::min(i_mu + 1, max_color)], mu - i_mu);
}