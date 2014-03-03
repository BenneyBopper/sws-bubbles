#include "sws.hpp"

float interpolate(std::vector<float> &array, float x){
	const int X = (int)x;
	const float d = x-X;
	return  (1-d)*array[X] + d*array[X+1];
}


void advectArray(std::vector<float> &array, int velocityType) { 
	for (int i=1;i<SIZE-1;i++) {
		const int index = i;
		// distinguish different cases to interpolate the velocity
		float u = 0; 
		switch(velocityType) {
			case 0: // heights
				u += (vel_x[index] + vel_x[index+1]) * 0.5;
				break;
			case 1: // x velocity
				u += vel_x[index];
				break;
			default: // invalid
				exit(1);
		}

		// backtrace position
		float srcpi = (float)i - u * dt * dxInv;

		// clamp range of accesses
		if(srcpi < 0.0) srcpi = 0.0;
		if(srcpi > SIZE - 1.0) srcpi = SIZE - 1.0;


		// interpolate source value
		temp[index] = interpolate(array, srcpi);
	}

	// copy back...
	for (int i=1;i<SIZE-1;i++){
		int index = i;
		array[index] = temp[index];
	}
}

void updateHeight() {
	// update heights
	for (int i=1;i<SIZE-1;i++) {
		int index = i;
		float dh = -0.5 * h[index] * dxInv * (vel_x[index+1] - vel_x[index]);
		h[index] += dh * dt;
	}
}

void updateVelocities() { 
	// accelerate x velocity
	for (int i=1;i<SIZE-1;i++){
		int index = i;
		vel_x[index] += gravity * dt * dxInv * (h[index] - h[index-1]);
	} 
}

void setBoundary() {
	// set reflecting boundaries
	h[0] = h[1];
	h[SIZE-1] = h[SIZE-2];
}

// run the solver...
int main(int argc, char *argv[])
{
	std::cout << "Initialising..." << std::endl;

	// allocate memory
	vel_x.resize(SIZE); 
	temp.resize(SIZE);
	h.resize(SIZE);

	// initialize grid
	for (int i=0;i<SIZE;i++){
		int index = i;
		vel_x[index] = 0.0;
		temp[index] = 0.0;

		// default water height 1
		h[index] = 1.0;
	}
					
	std::cout << "Starting simulation..." << std::endl;
	
	float simulationTime = 0.0;
	int  simulationStep = 0;

	while (simulationTime < END_TIME) 
	{
		std::cout << "Step " << simulationStep << std::endl;

		advectArray( h ,0 );
		advectArray( vel_x ,1 );

		updateHeight();
		updateVelocities();
		setBoundary();
		// addRandomDrop();

		simulationTime += dt;
		simulationStep++;
		h[3] = 1.5;

		// should we write an image for this step?
		if ( (simulationStep % RENDERSTEPS)== 0) {
			
		}

		std::cout << std::endl;
	}

	for (int i=0;i<SIZE;i++){
			std::cout << h[i] << " ";
	}
					
	std::cout << "Done." << std::endl;
	return 0;
}