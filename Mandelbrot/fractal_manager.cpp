#include "fractal_manager.h"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>

#include "farm.h"
#include "mandelbrot_task.h"
#include "multibrot_task.h"

FractalManager::FractalManager(sf::Window* window) :
	window_(window),
	width_(window_->getSize().x),
	height_(window_->getSize().y),
	max_iterations_(128),
	zoom_iter_add_(max_iterations_),
	mode_(1), // mandelbrot is 1, multibrot is 2
	lerping_(false),
	l(DEF_L),
	r(DEF_R),
	t(DEF_T),
	b(DEF_B),
	view_width_(std::abs(r - l)),
	view_height_(std::abs(t - b)),
	file_name_("output"),
	frames_done_(0),
	num_frames_(20),
	pos_x_(0),
	pos_y_(0),
	zoom_time_(0.01)

{
	// sets mandelbrot size to match window aspect ratio
	ratio_ = double(width_) / double(height_);

	l *= ratio_;
	r *= ratio_;

	updateViewSize();

	fractal_ = new Fractal(width_, height_);
	colourManager_ = new ColourManager();
}

FractalManager::~FractalManager()
{
	delete fractal_;
	fractal_ = NULL;

	delete colourManager_;
	colourManager_ = NULL;
}

void FractalManager::update()
{
	if (lerping_) {
		runAnimation(num_frames_, 1.0, zoom_time_);
	}
}

void FractalManager::runFarm()
{
	// Start timing
	auto start = clockStart();

	Farm farm;

	const double num_segments = height_;
	const double slice = (double)height_ / num_segments;

	for (int i = 0; i < num_segments; i++) {
		switch (mode_)
		{
		case 1:
			farm.add_task(new MandelbrotTask(fractal_->getImage(), l, r, t, b, i * slice, i * slice + slice, max_iterations_, colourManager_->current_scheme, width_, height_, *colourManager_->getSchemesVec()));
			break;

		case 2:
			farm.add_task(new MultibrotTask(fractal_->getImage(), l, r, t, b, i * slice, i * slice + slice, max_iterations_, colourManager_->current_scheme, width_, height_, *colourManager_->getSchemesVec()));
			break;
		}
	}

	farm.run();
	fractal_->update(lerping_, file_name_.c_str());

	// Stop timing
	auto time_taken = clockStop(start);

	updateViewSize();

	std::cout << std::setprecision(20) <<
		"Mode: " << mode_ <<
		"\nLeft X: " << l <<
		"\nBottom Y: " << b <<
		"\nWidth: " << view_width_ <<
		"\nHeight: " << view_height_ <<
		"\nIterations: " << max_iterations_ <<
		"\nTheme: " << colourManager_->current_scheme <<
		"\nTime taken: " << time_taken << "ms." <<
		std::endl << std::endl;
}

// Runs animation and saves frames to file, frames = num of frames in animation, m_t is how much to move lerp per frame, z_t is how much to zoom lerp per frame
void FractalManager::runAnimation(int frames, double m_t, double z_t)
{
	// creates frames for an animation and saves to output folder
	if (frames_done_ < frames) {
		if (frames_done_ == 0) {

			pos_x_ = sf::Mouse::getPosition(*window_).x / (double)width_ * view_width_ + l;
			pos_y_ = sf::Mouse::getPosition(*window_).y / (double)height_ * -view_height_ + t;

			int u_i;
			std::cout << "\nWould you like to use mouse location? (0 = no, 1 = yes)\n>> ";
			std::cin >> u_i;

			if (u_i == 0) {
				std::cout << "Enter the X position of your desired location\n>> ";
				std::cin >> pos_x_;
				std::cout << "Enter the Y position of your desired location\n>> ";
				std::cin >> pos_y_;
			}

			std::cout << "\nEnter the amount of frames for the animation\n>> ";
			std::cin >> num_frames_;

			std::cout << "\nEnter how much you want to zoom in per frame (recommended 0-100)\n>> ";

			double u_input;
			std::cin >> u_input;
			zoom_time_ = u_input / 150; // will be used in lerp function

			std::cout << "\nEnter the filename\n>> ";
			std::cin >> file_name_;

			start_animation_ = clockStart(); // start timer

			reset(); // go to start
		}
		lerpToPos(pos_x_, pos_y_, m_t, z_t); // lerp to pos
		frames_done_++; // incrase frames done
	}
	else {
		
		auto time_taken = clockStop(start_animation_);

		std::cout << "Total time taken for animation: " << time_taken << "ms.\n\n";

		std::ofstream output_file;
		output_file.open("output/" + file_name_ + ".txt", std::ios::app); // Open file with append flag, so we dont overwrite

		output_file << time_taken << std::endl;

		lerping_ = false;
		frames_done_ = 0;
	}
}

// Lerps the view rectangles edges towards eachother and lerps to position x y
void FractalManager::lerpToPos(double x, double y, double m_t, double z_t)
{
	// x, y is the center of the new view rectangle, m_t is the movement lerp "time" (0-1), z_t is the zoom lerp "time" (0-1), z_t can be negative to zoom out.

	//std::cout <<  "\n\ndouble centre_x = " << x << "; double centre_y = " << y << ";" << std::endl;
	std::cout << "Lerping to pos: " << x << ", " << y << std::endl;

	// zoom in (lerp the edges toward eachother based on z_t)
	l = std::lerp(l, r, z_t);
	r = std::lerp(r, l, z_t);

	t = std::lerp(t, b, z_t);
	b = std::lerp(b, t, z_t);

	updateViewSize();

	// move to new view rectangle (lerp the edges toward their new edge centered around x, y, based on m_t)

	l = std::lerp(l, x - view_width_ / 2, m_t);
	r = std::lerp(r, x + view_width_ / 2, m_t);

	t = std::lerp(t, y + view_height_ / 2, m_t);
	b = std::lerp(b, y - view_height_ / 2, m_t);

	runFarm();

	zoom_iter_add_ += (double)100 * z_t;
	max_iterations_ = (int)zoom_iter_add_; // increase detail (max_iterations) as we zoom in, and decrease as we zoom out
	if (max_iterations_ <= 0) {
		max_iterations_ = 10;
	}
}

// Updates view width/height variables
void FractalManager::updateViewSize()
{
	// update view size variables since these will change everytime we move/zoom
	view_width_ = std::abs(r - l);
	view_height_ = std::abs(t - b);
}

void FractalManager::handleMouseInput()
{
	// convert click location on window to coordinates inside the mandelbrot square and then lerp to it
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		lerpToPos(sf::Mouse::getPosition(*window_).x / (double)width_ * view_width_ + l, sf::Mouse::getPosition(*window_).y / (double)height_ * -view_height_ + t, 1.0, 0.3);
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		lerpToPos(sf::Mouse::getPosition(*window_).x / (double)width_ * view_width_ + l, sf::Mouse::getPosition(*window_).y / (double)height_ * -view_height_ + t, 1.0, -0.3);
	}
}

void FractalManager::handleScrollInput(float delta)
{
	// delta is how much scrollwheel moved, positive is up, negative is down
	max_iterations_ += delta * 10;
	zoom_iter_add_ = max_iterations_;
	runFarm();
}

void FractalManager::handleKeyboardInput()
{
	bool redraw = colourManager_->handleInput();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
		mode_ = 1;
		redraw = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
		mode_ = 2;
		redraw = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
		reset();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
		if (lerping_ == false) {
			lerping_ = true;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		window_->close();
	}

	if (redraw) runFarm(); // only run farm if necessary
}

void FractalManager::reset()
{
	// reset to initial square
	l = (DEF_L * ratio_); r = (DEF_R * ratio_); t = DEF_T; b = DEF_B;
	max_iterations_ = 128;
	zoom_iter_add_ = max_iterations_;

	updateViewSize();
	runFarm();
}