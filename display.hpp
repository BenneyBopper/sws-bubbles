#ifndef __DISPLAY_HPP__
#define __DISPLAY_HPP__

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <cmath>
#include <iostream>
#include <vector>

#ifdef _WIN32
	#undef main
#endif

class Display {
private:
	SDL_Window * window;	//SDL window
	SDL_Renderer * renderer;	//SDL renderer
	int _x, _y, _z;

public:
	bool running;	//used for running checks

	Display ()	 	//constructor
		: window(NULL), renderer(NULL), running(false) {}
	~Display () {	//destructor(destroys the window)
		if (window != NULL) {
			SDL_DestroyWindow(window);
			window = NULL;
		}
	}
	void getWindowSize(int *width, int *height);	//gets current window size
	void setWindowSize(int width, int height);		//sets size of window
	void init(int x=100, int y=100, int z=100, int w=500, int h=500,
		   std::string winame = "Simulation."); //Initialize the display
	void initGL();	//Initialize OpenGL
	void eventHandler(SDL_Event &event);	//SDL event handler
	void clearScreen();	//Clears the screen
	void render();	//Swaps frames to render the scene

	//Draws a circle/sphere
	void drawCircle( float cx, float cy, float cz, float r, int num_segments );

	//Draws a quad
	void drawQuad(float x, float y, float w, float h) const;
};

#endif /* __DISPLAY_HPP__ */