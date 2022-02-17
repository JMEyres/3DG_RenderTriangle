#ifndef _TEXTURING_H_
#define _TEXTURING_H_

#include "../../wogl/wogl.h"
#include "../example.h"

struct texturing : public example
{
private:
	GLuint positionsVboId = 0;
	GLuint textCoordsVbo = 0;
	GLuint vaoId = 0;
	GLuint programId = 0;
	GLuint texture = 0;
	GLint uProj = 0;
	GLint uModel = 0;
	float angle = 0;

public:
	// Polymorphic example run override
	void example::Run(SDL_Window* _window) override;

	// Instantiation
	texturing();
	~texturing();
};

#endif _TEXTURING_H_