#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include<stdexcept>

int main()
{
	SDL_Window *window = SDL_CreateWindow("3D Game",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		800, 600, SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL);

	bool quit = false;

	float maxFrame = 60.0f;
	float frameCount = 0.0f;

	const GLfloat positions[] = {
		0.0f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f
	};

	const GLchar* vertexShaderSrc =
		"attribute vec3 in_Position;			" \
		"										" \
		"void main()							" \
		"{										" \
		"gl_Position = vec4(in_Position,1.0);	" \
		"}										";
	
	const GLchar* fragmentShaderSrc =
		"void main()							" \
		"{										" \
		"gl_FragColor = vec4(0,0,0,1);			" \
		"}										";

	if (!SDL_GL_CreateContext(window))
	{
		throw std::runtime_error("Failed to create OpenGL Context");
	}
	if (glewInit() != GLEW_OK)
	{
		throw std::runtime_error("Failed to initialise glew");
	}

	GLuint positionsVboId = 0;

	// Create a new VBO on the GPU and bind it
	glGenBuffers(1, &positionsVboId);

	if (!positionsVboId)
	{
		throw std::runtime_error("!positionsVboId");
	}

	glBindBuffer(GL_ARRAY_BUFFER, positionsVboId);

	// Upload a copy of the data from memory into the new VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	// Reset the state
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint vaoId = 0;

	// Create a new VAO on the GPU and bind it
	glGenVertexArrays(1, &vaoId);

	if (!vaoId)
	{
		throw std::runtime_error("!vaoId");
	}

	glBindVertexArray(vaoId);

	// Bind the position VBO, assign it to position 0 on the bound VAO
	// and flag it to be used
	glBindBuffer(GL_ARRAY_BUFFER, positionsVboId);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		3 * sizeof(GLfloat), (void*)0);

	glEnableVertexAttribArray(0);

	// Reset the state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Create a new vertex shader, attach source code, compile it and
	// check for errors

	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, &vertexShaderSrc, NULL);
	glCompileShader(vertexShaderId);
	GLint success = 0;
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		throw std::runtime_error("!success (vertex)");
	}

	// Create a new fragment shader, attach source code, compile it and
	// check for errors

	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId,1,&fragmentShaderSrc, NULL);
	glCompileShader(fragmentShaderId);
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		std::runtime_error("!success (fragment)");
	}

	// Create new shader program and attach our shader objects
	GLuint programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);

	// Ensure the VAO "position" attribute stream gets set as the first position
	// during the link
	glBindAttribLocation(programId, 0, "in_Position");

	// Perform the link and check for failure
	glLinkProgram(programId);
	glGetProgramiv(programId, GL_LINK_STATUS, &success);

	if (!success)
	{
		throw std::runtime_error("!success (link)");
	}

	// Detach and destroy the shader objects. These are no longer needed
	// because we now have a complete shader program
	glDetachShader(programId, vertexShaderId);
	glDeleteShader(vertexShaderId);
	glDetachShader(programId, fragmentShaderId);
	glDeleteShader(fragmentShaderId);

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

		frameCount++;
		if (frameCount > maxFrame)
		{
			frameCount = 0.0f;
		}

		float change = frameCount / maxFrame;

		glClearColor(change, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		

		// Instruct OpenGL to use our shader program and our VAO
		glUseProgram(programId);
		glBindVertexArray(vaoId);

		// Draw 3 vertices (a triangle)
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Reset the state
		glBindVertexArray(0);
		glUseProgram(0);
		
		SDL_GL_SwapWindow(window);
	}
	return 0;
}