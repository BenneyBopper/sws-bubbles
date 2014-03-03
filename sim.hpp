#ifndef __SIM_HPP__
#define __SIM_HPP__

#include <SDL2/SDL.h>
#include <limits>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include "display.hpp"

#define MODEL "1D Shallow Water Simulation"
#define DIM_X 100
#define DIM_Y 3

#define WINDOW_X 512
#define WINDOW_Y 512

class Sim {
private:
	Display &display;
	int x, y;

	const int SIZE = 100;
	const int RENDERSTEPS = 0;
	const int END_TIME = 25;

	// constants
	const float gravity = -10.0;  // normal acceleration a_n
	const float dt = 10.0 / (float)SIZE;       // time step size
	const float domainSize = 50.0; // size of the domain
	const float dx = domainSize / (float)SIZE;  
	const float dxInv = 1.0/dx;   // save some division later on

	// main arrays velocity, height, and temporary storage
	std::vector<float> vel_x, vel_y, temp, h;

	//Handle key inputs
	void eventHandler();

	//Draw + Render Screen
	void draw();

	float interpolate(std::vector<float> &array, float x);

	void advectArray(std::vector<float> &array, int velocityType);

	void updateHeight();

	void updateVelocities();

	void setBoundary();

public:
	Sim(Display &display, int x=DIM_X, int y=DIM_Y) : display(display), x(x), y(y) {}
	
	void init(unsigned int seed = time(NULL));

	void run();
};

#endif /* __SIM_HPP__ */