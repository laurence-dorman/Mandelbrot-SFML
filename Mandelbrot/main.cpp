#include <SFML/Graphics.hpp>

#include "mandelbrot.h"

const unsigned int width = 1280;
const unsigned int height = 960;

int main()
{
    sf::RenderWindow window(sf::VideoMode(width, height), "Mandelbrot");

	Mandelbrot mandelbrot(width, height);

	mandelbrot.compute_mandelbrot(-2.0, 1.0, 1.125, -1.125, 0, height);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(mandelbrot);
        window.display();
    }

    return 0;
}