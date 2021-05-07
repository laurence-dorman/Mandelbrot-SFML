#pragma once

#include <SFML/Window.hpp>
#include <chrono>

#include "fractal.h"
#include "colour_manager.h"


class FractalManager
{

public:
	FractalManager(Fractal* f, sf::Window* window);
	~FractalManager();

	void update();

	void runFarm();
	void reset();

	void handleMouseInput();
	void handleScrollInput(float delta);
	void handleKeyboardInput();

private:
	Fractal* fractal_;
	ColourManager* colourManager_;
	sf::Window* window_;

	void updateViewSize();
	void lerpToPos(double x, double y, double m_t, double z_t);
	void runAnimation(int frames, double m_t, double z_t);

	double l, r, t, b;

	unsigned int width_, height_;
	double view_width_, view_height_;
	int max_iterations_;
	double zoom_iter_add_;
	int mode_;
	std::string file_name_;

	// vars for animations
	bool lerping_;
	int frames_done_;
	int num_frames_;
	double pos_x_, pos_y_;
	double zoom_time_;

	typedef std::chrono::high_resolution_clock the_clock;

	the_clock::time_point start_animation_;
};

