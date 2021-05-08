#pragma once

#include <SFML/Window.hpp>
#include <chrono>

#include "fractal.h"
#include "colour_manager.h"

typedef std::chrono::high_resolution_clock the_clock;

// default values for l r t b
#define DEF_L -2.0
#define DEF_R 1.0
#define DEF_T 1.5
#define DEF_B -1.5

class FractalManager
{
public:
	FractalManager(sf::Window* window);
	~FractalManager();

	void update();

	void runFarm();
	void reset();

	// Input functions
	void handleMouseInput();
	void handleScrollInput(float delta);
	void handleKeyboardInput();

	// Returns pointer to fractal object
	Fractal* getFractal() { return fractal_; }

private:
	Fractal* fractal_;
	ColourManager* colourManager_;
	sf::Window* window_;

	void updateViewSize();
	void lerpToPos(double x, double y, double m_t, double z_t);
	void runAnimation(int frames, double m_t, double z_t);

	// clockStart function, returns time_point of the_clock - used for timing
	the_clock::time_point clockStart() { return the_clock::now();}

	// clockStop function, takes in time_point start clock start value, returns difference between current time
	long long clockStop(the_clock::time_point start) { return duration_cast<milliseconds>(the_clock::now() - start).count(); }

	// variables for fractal view square
	double l, r, t, b; // left, right, top, bottom
	double view_width_, view_height_;

	// variables for interacting with fractal
	int max_iterations_; // maximum fractal iterations
	double zoom_iter_add_; // amount to zoom in 

	// variables for fractal (sprite) size
	unsigned int width_, height_;
	
	int mode_; // fractal mode
	std::string file_name_; // if user runs animation this will be the file name of generated png's followed by its index value

	// vars for animations
	bool lerping_;
	int frames_done_;
	int num_frames_;
	double pos_x_, pos_y_;
	double zoom_time_;

	// For scaling mandelbrot based on window size
	double ratio_;

	the_clock::time_point start_animation_;

};

