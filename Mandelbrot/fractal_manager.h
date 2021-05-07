#pragma once

#include <SFML/Window.hpp>
#include <chrono>

#include "fractal.h"
#include "colour_manager.h"

typedef std::chrono::high_resolution_clock the_clock;

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

};

