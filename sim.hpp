#ifndef __SIM_HPP__
#define __SIM_HPP__

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <limits>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include "display.hpp"
#include "bubble.hpp"

#define MODEL "1D Shallow Water Simulation"
#define DIM_X 2
#define DIM_Y 3

#define WINDOW_X (DIM_X*200)
#define WINDOW_Y (DIM_Y*200)

#define vec glm::dvec2

class Sim {
private:
	Display &display;
	int x, y;

	const int SIZE = 100;
	const int RENDERSTEPS = 0;
	const int END_TIME = 25;
	
	// constants
	const float gravity = -10.0;  // normal acceleration a_n
	const float dt = 0.001; //10.0 / (float)SIZE;       // time step size 0.001
	const float domainSize = 1.0; // size of the domain
	const float dx = domainSize / (float)SIZE;  
	const float dxInv = 1.0/dx;   // save some division later on

	// main arrays velocity, height, and temporary storage
	std::vector<float> vel_x, vel_y, temp, h;

	// bubble storage
	std::vector<Bubble> bubbles;

	// bubble collision types
	enum collisionType {
		NONE,
		WATER,
		WALL,
		BUBBLE
	};
	bool checkCollision;
	vec bounds; 

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