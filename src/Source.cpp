#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "examples/example.h"
#include "examples/texturing/Texturing.h"
#include<stdexcept>
#include<glm/glm.hpp>
#include<glm/ext.hpp>

int main()
{
	int width = 0;
	int height = 0;

	SDL_Window* window = SDL_CreateWindow("3D Game",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	if (!SDL_GL_CreateContext(window) || glewInit() != GLEW_OK) { throw std::runtime_error("Context failed to initialise"); }

	bool quit = false;

	example* program = new texturing{};

	while (!quit)
	{
		SDL_Event event = { 0 };
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		SDL_GetWindowSize(window, &width, &height);
		glViewport(0, 0, width, height);

		program->Run(window);

		SDL_GL_SwapWindow(window);
	}
	return 0;
}