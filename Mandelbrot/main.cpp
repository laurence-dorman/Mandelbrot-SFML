#include <SFML/Graphics.hpp>

#include "mandelbrot.h"
#include <iostream>

const unsigned int width = 1280;
const unsigned int height = 960;

sf::Rect<long double> view(-2.0, -1.125, 3.0, 2.25);

sf::RenderWindow* window;

void handle_keyboard_input()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        view.left += 0.128/2;
        view.top += 0.096/2;
        view.width -= 0.128;
        view.height -= 0.096;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        view.left -= 0.128/2;
        view.top -= 0.096/2;
        view.width += 0.128;
        view.height += 0.096;
    }
}

void handle_mouse_input()
{
    //view.left = (double)sf::Mouse::getPosition(*window).x / (double)width;
    //view.top = (double)sf::Mouse::getPosition(*window).y / (double)height;
    //std::cout << view.left << std::endl << view.top << std::endl << std::endl;
}

int main()
{
    //sf::RenderWindow window(sf::VideoMode(width, height), "Mandelbrot");
    window = new sf::RenderWindow(sf::VideoMode(width, height), "Mandelbrot");

	Mandelbrot mandelbrot(width, height);

	mandelbrot.compute_mandelbrot(view, 0, height);

    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();

            if (event.type == sf::Event::KeyPressed) 
            {
                handle_keyboard_input();
                mandelbrot.do_mandelbrot(view);
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                handle_mouse_input();
                mandelbrot.do_mandelbrot(view);
            }
        }

        window->clear();
        window->draw(mandelbrot);
        window->display();
    }

    return 0;
}