#ifndef _EXAMPLE_H_
#define _EXAMPLE_H_

#include <SDL2/SDL.h>
#include <GL/glew.h>

// Example template
struct example
{
	virtual void Run(SDL_Window* _window) = 0;
};

#endif _EXAMPLE_H_
