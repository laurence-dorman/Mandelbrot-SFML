#include <SFML/Graphics.hpp>
#include <complex>
#include <math.h>

using std::complex;

const unsigned int width = 1280;
const unsigned int height = 960;
const unsigned int max_iterations = 100;

sf::Image buffer;

sf::Color getColour(int i) {
	int colourVal = (255.f / (float)max_iterations)*i;
	
	int blueAdd = (std::abs(colourVal - 255) < 50) ? std::abs(colourVal - 255) : 50;

	return sf::Color(colourVal, colourVal, colourVal + blueAdd);
}

// Render the Mandelbrot set into the image array.
// The parameters specify the region on the complex plane to plot.
void compute_mandelbrot(double left, double right, double top, double bottom, double y_start, double y_end)
{
	for (int y = y_start; y < y_end; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			// Work out the point in the complex plane that
			// corresponds to this pixel in the output image.
			complex<double> c(left + (x * (right - left) / width),
				top + (y * (bottom - top) / height));

			// Start off z at (0, 0).
			complex<double> z(0.0, 0.0);

			// Iterate z = z^2 + c until z moves more than 2 units
			// away from (0, 0), or we've iterated too many times.
			int iterations = 0;
			while (abs(z) < 2.0 && iterations < max_iterations)
			{
				z = (z * z) + c;

				++iterations;
			}

			if (iterations == max_iterations)
			{
				// z didn't escape from the circle.
				// This point is in the Mandelbrot set.
				buffer.setPixel(x, y, sf::Color(0, 0, 0)); // black
			}
			else
			{
				// z escaped within less than MAX_ITERATIONS
				// iterations. This point isn't in the set.
				buffer.setPixel(x, y, getColour(iterations)); // get colour based on iterations
			}
		}
	}
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(width, height), "Mandelbrot");

    sf::Texture texture;

	texture.create(width, height);

	buffer.create(width, height, sf::Color(0, 0, 0));

	texture.update(buffer);

	sf::Sprite sprite;
	sprite.setTexture(texture);

	compute_mandelbrot(-2.0, 1.0, 1.125, -1.125, 0, height);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

		
		texture.update(buffer);

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}