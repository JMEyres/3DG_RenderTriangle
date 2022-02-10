#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <GL/glew.h>
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

	bool quit = false;

	float maxFrame = 60.0f;
	float frameCount = 0.0f;
	float angle = 0.0f;

	const GLfloat positions[] = {
		0.0f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f
	};

	const GLfloat colors[] = {
		1.0f,0.0f,0.0f,1.0f,
		0.0f,1.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,0.0f
	};

	const GLchar* vertexShaderSrc =
		"attribute vec3 a_Position;				" \
		"attribute vec4 a_Color;				" \
		"uniform mat4 u_Projection;				" \
		"uniform mat4 u_Model;					" \
		"										" \
		"varying vec4 v_Color;					" \
		"										" \
		"void main()							" \
		"{										" \
		"gl_Position = u_Projection * u_Model * vec4(a_Position,1.0);	" \
		"v_Color = a_Color;						" \
		"}										" \
		"										";

	const GLchar* fragmentShaderSrc =
		"uniform vec4 u_Color;					" \
		"varying vec4 v_Color;					" \
		"										" \
		"void main()							" \
		"{										" \
		"gl_FragColor = v_Color*u_Color;		" \
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

	// Color VBO
	GLuint colorsVboId = 0;

	// Create a colors VBO on the GPU and bind it
	glGenBuffers(1, &colorsVboId);

	if (!colorsVboId)
	{
		throw std::runtime_error("!colorsVboId");
	}

	glBindBuffer(GL_ARRAY_BUFFER, colorsVboId);

	// Upload a copy of the data from memory into the new VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

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

	// Bind the color VBO, assign it to position 1 on the bound VAO
	// and flag it to be used
	glBindBuffer(GL_ARRAY_BUFFER, colorsVboId);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
		4 * sizeof(GLfloat), (void*)0);

	glEnableVertexAttribArray(1);

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
	glShaderSource(fragmentShaderId, 1, &fragmentShaderSrc, NULL);
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
	glBindAttribLocation(programId, 0, "a_Position");

	glBindAttribLocation(programId, 1, "a_Color");

	// Perform the link and check for failure
	glLinkProgram(programId);
	glGetProgramiv(programId, GL_LINK_STATUS, &success);

	if (!success)
	{
		throw std::runtime_error("!success (link)");
	}

	// Store location of the color uniform and check if successfully found
	GLint colorUniformId = glGetUniformLocation(programId, "u_Color");
	// Find uniform locations
	GLint modelLoc = glGetUniformLocation(programId, "u_Model");
	GLint projectionLoc = glGetUniformLocation(programId, "u_Projection");

	if (colorUniformId == -1)
	{
		throw std::runtime_error("!colorUniformId");
	}


	// Bind the shader to change the uniform, set the uniform and reset state
	glUseProgram(programId);
	
	glUseProgram(0);

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

		SDL_GetWindowSize(window, &width, &height);
		glViewport(0, 0, width, height);

		frameCount++;
		if (frameCount > maxFrame)
		{
			frameCount = 0.0f;					//Used for animating color
		}

		float change = frameCount / maxFrame;

		glClearColor(0.4235294117647059f, 0.47843137254901963f, 0.47843137254901963f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// Instruct OpenGL to use our shader program and our VAO
		glUseProgram(programId);
		glBindVertexArray(vaoId);

		glUniform4f(colorUniformId, 1, 1, 1, 1);

		// Prepare the perspective projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)800 / (float)600, 0.1f, 100.0f);
		// Model matrix perspective
		glm::mat4 pModel(1.0f);
		pModel = glm::translate(pModel, glm::vec3(0, 0, -2.5f));
		pModel = glm::rotate(pModel, glm::radians(angle), glm::vec3(1, 1, 1));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(pModel));

		// Projection perspective
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE,
			glm::value_ptr(projection));

		// Draw 3 vertices (a triangle)
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Prepare the ortho projection matrix
		glm::mat4 oProjection = glm::perspective(glm::radians(45.0f),
			(float)800 / (float)600, 0.1f, 100.0f);

		// Prepare the model matrix
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0, 0, -2.5f));
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1, 1, 1));

		// Increase the float angle so next fram the triangle rotates further
		angle += 0.5f;

		// Prepare the othorgraphic projection matrix (reusing the variable)
		oProjection = glm::ortho(0.0f, (float)800, 0.0f,
			(float)600, 0.0f, 1.0f);

		// Prepare model matrix. The scale is important because now our triangle
		// would be the size of a single pixel when mapped to an orthographic
		// projection.
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(100, 600 - 100, 0));
		model = glm::scale(model, glm::vec3(100, 100, 1));

		// Make sure the current program is bound

		// Upload the model matrix
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		
		// Upload the projection matrix
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE,
			glm::value_ptr(oProjection));	

		// Draw shape as before

		// Draw 3 vertices (a triangle)
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Reset the state
		glBindVertexArray(0);
		glUseProgram(0);

		SDL_GL_SwapWindow(window);
	}
	return 0;
}