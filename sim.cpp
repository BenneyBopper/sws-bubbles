#include "sim.hpp"

void Sim::eventHandler() {
	SDL_Event event;
	//While there's an event to handle
	while (SDL_PollEvent(&event)) {
		// Pass the event down to the visualiser so it can react to it.
		display.eventHandler(event);

		switch (event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						display.running = false; 
						break;
					case SDLK_DOWN:
						h[49] += 0.1;
						break;
					case SDLK_LEFT:
						h[1] += 0.1;
						break;
					case SDLK_RIGHT:
						h[98] += 0.1;
						break;
					case SDLK_UP:
						bubbles.push_back(Bubble( 0.004, vec(0.2,0.0), vec(0,0)));
				}
				break;
		}
	}
}

void Sim::draw() {
	glColor3f(0.0,0.0,0.6);
	for (int i=1;i<SIZE-1;i++){
		display.drawQuad( i*dx, 0, 1*dx, h[i]);
		// std::cout << h[i] << " ";
	}
	glColor4f(0.8,0.8,0.8,0.5);
	for (std::vector<Bubble>::iterator b = bubbles.begin(); b != bubbles.end(); ++b) {
		display.drawCircle( b->p[0], b->p[1], 0, b->r, 32 );
	}
	display.render();
}

float Sim::interpolate(std::vector<float> &array, float x){
	const int X = (int)x;
	const float d = x-X;
	return  (1-d)*array[X] + d*array[X+1];
}

void Sim::advectArray(std::vector<float> &array, int velocityType) { 
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

void Sim::updateHeight() {
	// update heights
	for (int i=1;i<SIZE-1;i++) {
		int index = i;
		float dh = -0.5 * h[index] * /*dxInv **/ (vel_x[index+1] - vel_x[index]);
		h[index] += dh * dt;
	}
}

void Sim::updateVelocities() { 
	// accelerate x velocity
	for (int i=2;i<SIZE-1;i++){
		int index = i;
		vel_x[index] += gravity * dt * dxInv * (h[index] - h[index-1]);
	} 
}

void Sim::setBoundary() {
	// set reflecting boundaries
	h[0] = h[1];
	h[SIZE-1] = h[SIZE-2];

	/*vel_x[0] = 0.0;
	vel_x[1] = 0.0;
	vel_x[SIZE-1] = 0.0;*/
}

/////////////////////////////////////////////////////////////////////////////
void Sim::init(unsigned int seed) {
	srand(seed);
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

	bounds[0] = (SIZE-1)*dx;
	bounds[1] = DIM_Y;

	for (int i=0; i<5; i++) {
		bubbles.push_back(Bubble( 0.004, vec(0.2+ i*0.01,0.1), vec(0,0)));
	}

	// Initialise the display
	display.init(x,y,0,WINDOW_X,WINDOW_Y,MODEL);
}

void Sim::run() {
	std::cout << "Starting simulation..." << std::endl;
	float simulationTime = 0.0;
	int simulationStep = 0;
	double buoyancy = 0.0, /*liquid = 0.0,*/ drag = 0.0, vAbsRel = 0.0; //Forces on bubbles

	while (display.running) {
		display.clearScreen();
		eventHandler();

		// std::cout << "Step " << simulationStep << std::endl;
		// for(int s = 0; s < 10; s++) {
			//Update the positions of all bubbles
			for (std::vector<Bubble>::iterator b = bubbles.begin(); b != bubbles.end(); ++b) {
				b->update(dt);
			}
/*
			checkCollision = true;
			while (checkCollision == true){
				//Check what type of collision occured first
				collisionType collision = NONE;

				//First collision variables
				std::vector<Bubble>::iterator a, b; //Iterators for colliding bubbles
				vec da, db; //distance a,b
				double t = std::numeric_limits<double>::max(); //initialise timestep for maximum rewind!
				int dimen;	//dimension variable

				//Iterate through all bubbles
				std::vector<Bubble>::iterator i = bubbles.begin();
				while (i != bubbles.end()) {

					// Check wall collisions!
					for (int d=0; d<2; d++) { //check in every dimensions
						double dist = bounds[d] - fabs(i->p[d]); //bound - bubbles position
						if (dist < i->r) {	//distance < radius
							double t_w = (i->r - dist) / fabs(i->v[d]);	//calc timestep at wall
							if (t_w < t) {	//if timestep at wall < timestep
								collision = WALL;
								dimen = d;
								t   = t_w;
								a   = i;
							}
						}
					}

					// Check bubble collisions!
					std::vector<Bubble>::iterator j = i+1; //Look at the next bubble
					while (j != bubbles.end()) {
						double dist = glm::distance(i->p, j->p); //distance from i to j

						if (dist < (i->r+j->r)) { // if less than the radius of i + j
							vec di = glm::normalize(j->p - i->p);	//normalize i-j positions
							vec dj = -di;	//negate di

							double t_ij = ((i->r+j->r) - dist) / (glm::dot(i->v,di) + glm::dot(j->v,dj));	// calc timestep at collision
							if (t_ij < t) {	//if timestep at collision < timestep
								collision = BUBBLE;
								t  = t_ij;
								a  = i;
								b  = j;
								da = di;
								db = dj;
							}
						}
						j++;
					}
					i++;
				}

				//Resolve collision of bubble a
				switch (collision) {
					case BUBBLE: {
						//Step back to when collision occured
						a->p -= t * a->v;
						b->p -= t * b->v;

						//Change velocity
						vec ua = a->v;
						vec ub = b->v;
						vec dotua = da*glm::dot(ua, da);
						vec dotub = db*glm::dot(ub, db);
						a->v = (ua - dotua + dotub*0.1);
						b->v = (ub - dotub + dotua*0.1);
						
						//Step forward to correct position
						a->p += t * a->v;
						b->p += t * b->v;
						checkCollision = true;
						break;
					}
					case WALL: {
						a->p -= t * a->v; //Step back to when collision occured
						a->v[dimen] = - a->v[dimen]; //Change velocity
						a->p += t * a->v; //Step forward to correct position
						checkCollision = true;
						break;
					}
					case NONE:
						checkCollision = false;
						break;
					default:
						checkCollision = false;
						break;
				}
			}
*/

//////////////////////////////////////////////////////////////
/// Shallow Water Calculations
//////////////////////////////////////////////////////////////
			
			advectArray( h ,0 );
			advectArray( vel_x ,1 );

			updateHeight();
			updateVelocities();
			setBoundary();

/////////////////////////////////////////////////////////////

			//collisions

			//all F[i] = (0,0)
			//Iterate through all bubbles
			std::vector<Bubble>::iterator i = bubbles.begin();
			while (i != bubbles.end()) {
				i->f[0] = 0;
				i->f[1] = 0;
				i++;
			}

			//First collision variables
			vec ab, ba, fa, fb;
			double f;

			//Iterate through all bubbles
			std::vector<Bubble>::iterator a = bubbles.begin();
			while (a != bubbles.end()) {
				for (int dim=0; dim<2; dim++) { //check both dimensions
					if (a->p[dim] > bounds[dim]) {
						a->p[dim] = bounds[dim];
					}
					double dist = (bounds[dim]) - fabs(a->p[dim]); //bound - bubbles position
					if (dist < a->r) {	//distance < radius
						f = 100000 * (a->r - dist);
						a->f[dim] -= f;
					}

					if (a->p[dim] < dx) {
						a->p[dim] = dx;
					}
					dist = fabs(a->p[dim]) - dx; //bound - bubbles position
					if (dist < a->r) {	//distance < radius
						f = 100000 * (a->r - dist);
						a->f[dim] += f;
					}
				}

				//Look at Bubble collisions
				std::vector<Bubble>::iterator b = a + 1; //Look at the next bubble
				while (b != bubbles.end()) {
					ab = glm::normalize(b->p - a->p);
					ba = glm::normalize(a->p - b->p);

					double d = glm::distance(a->p, b->p); //distance from a to b

					if (d < (a->r+b->r)) { // if less than the radius of a + b
						d = a->r+b->r - d;
						f = 75000 * d;
						fa = ba * f;
						fb = ab * f;
						a->f += fa;
						b->f += fb;
					}
					b++;
				}
				a++;
			}




			//
			i = bubbles.begin();
			while (i != bubbles.end()) {
				if(h[i->p[0]/dx] - i->p[1] > i->r) {
					buoyancy = i->area * 1000 - i->area * i->density; //makes it go up
				} else {
					i->p[1] = h[i->p[0]/dx] - i->r;
				}
				// std::cout << buoyancy << std::endl;
				// liquid = vel_x[i->p[0]/dx];

				vec vRel( i->v[0] - vel_x[i->p[0]/dx], i->v[1] );
				vAbsRel = std::sqrt( vRel[0]*vRel[0] + vRel[1]*vRel[1] );
				vec n(0,1);
				if(vAbsRel != 0.0) {
					n = vec( vRel[0]/vAbsRel, vRel[1]/vAbsRel );
				}
				

				 //std::cout << liquid << std::endl;
				drag = -0.5 * 1000 * (vAbsRel * vAbsRel) * 0.47 * i->area; //http://en.wikipedia.org/wiki/Drag_(physics)
				// std::cout << drag << std::endl;
				// std::cout << i->v[1] << std::endl;

				i->v[0] += (i->f[0] + n[0] * drag ) * dt;
				i->v[1] += (i->f[1] + buoyancy + ( n[1] * drag)) * dt;
				// i->v[1] = i->v[1] + ((buoyancy - drag)) * dt;
				// i->v[1] = 1.0;
				i++;
			}

			simulationTime += dt;
			simulationStep++;
		//}

		//Then draw it
		draw();
	}
}