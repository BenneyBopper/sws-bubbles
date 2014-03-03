#include "display.hpp"

void Display::getWindowSize(int *width, int *height) {
	SDL_GetWindowSize(window, width, height);
}

void Display::setWindowSize(int width, int height) {
	glViewport( (width-height)/2, 0, 
				(GLsizei) width>height?height:width, 
				(GLsizei) height>width?width:height );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	// Orthographic projection matrix (2D)
	glOrtho(0,_x-1,0,_y-1,0,_z-1);
	// glOrtho(-50,_x+50,-50,_y+50,-50,_z+50);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

void Display::init(int x, int y, int z, int w, int h, std::string winame) {
	//Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cout << SDL_GetError() << std::endl;
		return;
	}

	//Gather renderer information
	SDL_RendererInfo rendererInfo;
	//Create a new window
	window = SDL_CreateWindow( winame.c_str(), SDL_WINDOWPOS_UNDEFINED, 
		SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE 
	);
	//Gets info about a renderers context
	SDL_GetRendererInfo(renderer, &rendererInfo);
	//Creates a SDL openGL context
	SDL_GLContext context = SDL_GL_CreateContext(window);
	initGL();	//Initialise openGL settings
	_x = x;
	_y = y;
	_z = z;
	setWindowSize(w, h);	//Window size
	running = true;	//Make it run!
}

void Display::initGL() {
	// Enable smooth shading
	glShadeModel( GL_SMOOTH );
	// Depth buffer setup
	glClearDepth( 1.0f );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	// // Enable alpha
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Display::eventHandler(SDL_Event &event) {
	switch(event.type) {
		case SDL_QUIT:
			running = false;
			break;
		case SDL_WINDOWEVENT:
			switch (event.window.event) {
				//Takes care of window resizing
				case SDL_WINDOWEVENT_RESIZED:
					// cout << "Window resized to " << event.window.data1 << "x" << event.window.data2 << endl;
					setWindowSize(event.window.data1, event.window.data2);
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}

void Display::clearScreen() {
	glClearColor( 1.0f, 1.0f, 1.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glLoadIdentity();
}

void Display::render() {
	SDL_GL_SwapWindow(window);
}

/////////////////////////////////////////////////////////////
// void Display::drawCircle( float cx, float cy, float cz, float r, int num_segments) {
// 	if(!cz) cz = 0;
// 	float theta = 2 * M_PI / float(num_segments); 
// 	float tangetial_factor = tanf(theta);//calculate the tangential factor 
// 	float radial_factor = cosf(theta);//calculate the radial factor 
	
// 	float x = r;//we start at angle = 0 
// 	float y = 0; 
	
// 	// glBegin(GL_LINE_LOOP); 
// 	glBegin(GL_TRIANGLE_FAN); 
// 	for(int ii = 0; ii < num_segments; ii++) { 
// 		glVertex3f(x + cx, y + cy, cz);//output vertex 
		
// 		//calculate the tangential vector 
// 		//remember, the radial vector is (x, y) 
// 		//to get the tangential vector we flip those coordinates and negate one of them 

// 		float tx = -y; 
// 		float ty = x; 
		
// 		//add the tangential vector 
// 		x += tx * tangetial_factor; 
// 		y += ty * tangetial_factor; 
		
// 		//correct using the radial factor 
// 		x *= radial_factor;
// 		y *= radial_factor;
// 	} 
// 	glEnd(); 
// }

void Display::drawQuad(float x, float y, float w, float h) const {
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x, y, 0);
	glBegin( GL_QUADS );
		// Corner-to-corner Quad
		// glVertex3f( 0.0f, 0.0f, 0.0f );
		// glVertex3f( 0.0f, h,    0.0f );
		// glVertex3f( w,    h,    0.0f );
		// glVertex3f( w,    0.0f, 0.0f );
		glVertex3f( w,    0.0f, 0.0f );
		glVertex3f( w, 	  h,    0.0f );
		glVertex3f( 0.0f, h,    0.0f );
		glVertex3f( 0.0f, 0.0f, 0.0f );
		// // Centered Quad
		// w/=2;
		// h/=2;
		// glVertex3f( -w, -h, 0.0f );
		// glVertex3f( -w,  h, 0.0f );
		// glVertex3f(  w,  h, 0.0f );
		// glVertex3f(  w, -h, 0.0f );
	glEnd( );
	glPopMatrix();
}

