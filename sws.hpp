#ifndef __SWS_HPP__
#define __SWS_HPP__

#include <iostream>
// #include <stdlib.h>
#include <cmath>
#include <vector>

// global parameters
const int SIZE = 20;

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

//void addRandomDrop();

float interpolate(std::vector<float> &array, float x);

void advectArray(std::vector<float> &array, int velocityType);

void updateHeight();

void updateVelocities();

void setBoundary();

#endif /* __SWS_HPP__ */