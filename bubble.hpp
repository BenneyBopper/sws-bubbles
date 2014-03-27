#ifndef __BUBBLE_HPP__
#define __BUBBLE_HPP__

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/vec2.hpp>

#define vec glm::dvec2

const double PI = 3.141592653589793238462;

class Bubble {
public:
	double r, volume, area, density; //radius, volume and density
	vec p, v, f; //position, velocity and force

	Bubble(double rad=1, vec pos=vec(), vec vel=vec(), double d=1.2041) { //Constructor
		r = rad;
		p = pos;
		v = vel;
		density = d;
		updateVolume();
		updateArea();
	}

	void update(double dt) {	//Update particle
		p += v * dt;
	}

	void updateVolume() {
		volume = PI * r * r;
	}
	void updateArea() {
		area = 2 * r;
	}
};

#endif /* __BUBBLE_HPP__ */