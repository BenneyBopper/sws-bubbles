#include <iostream>

#include "display.hpp"
#include "sim.hpp"

int main(int argc, char **argv) {
	Display display;
	Sim sim(display);
	sim.init();
	sim.run();
	return 0;
}