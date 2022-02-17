#pragma once

#include <STB/image.h>
#include<stdexcept>
#include <cmath>
#include<glm/glm.hpp>
#include<glm/ext.hpp>

#include "Texturing.h"
#include "../../wogl/wogl.h"

void texturing::Run(SDL_Window* _window)
{
	// Clear buffer with solid color
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

	// Bind shader and VAO
	glUseProgram(programId);
	glBindVertexArray(vaoId);

	// Bind texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// prepare perspective projection matrix
	float w, h;
	SDL_GetWindowSize(_window, (int*)&w, (int*)&h);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), w / h, 0.1f, 100.0f);

	// prepare model matrix
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(0, 0, -2.5));
	model = glm::rotate(model, glm::radians(angle += 0.8f), glm::vec3(1.0f, 1.0f, 1.0f));

	// Upload matrices to respective uniforms
	glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));

	// Draw quad
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Unbind everything
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

texturing::texturing()
{
	// Get vertex positions to add to VBO
	const GLfloat postitions[] = {
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f
	};
	positionsVboId = wo::CreateVBO(sizeof(postitions), postitions);

	// Texture coords
	const GLfloat texcoords[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f
	};
	textCoordsVbo = wo::CreateVBO(sizeof(texcoords), texcoords);

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

	// Bind the texture VBO, assign it to position 1 on the bound VAO
	// and flag it to be used
	glBindBuffer(GL_ARRAY_BUFFER, textCoordsVbo);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
		2 * sizeof(GLfloat), (void*)0);

	glEnableVertexAttribArray(1);

	// Reset the state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	GLuint vertexShader = wo::CompileShader(GL_VERTEX_SHADER, wo::ReadFile("./src/examples/texturing/shader.vert").c_str());
	GLuint fragmentShader = wo::CompileShader(GL_FRAGMENT_SHADER, wo::ReadFile("./src/examples/texturing/shader.frag").c_str());

	programId = glCreateProgram();
	glAttachShader(programId, vertexShader);
	glAttachShader(programId, fragmentShader);

	// Ensure the VAO "position" attribute stream gets set as the first position
	// during the link
	glBindAttribLocation(programId, 0, "a_Position");

	glBindAttribLocation(programId, 1, "a_TexCoord");

	// Perform the link and check for failure
	glLinkProgram(programId);
	GLint success = 0;
	glGetProgramiv(programId, GL_LINK_STATUS, &success);

	if (!success)
	{
		throw std::runtime_error("!success (link)");
	}

	// Find uniform locations
	uModel = glGetUniformLocation(programId, "u_Model");
	uProj = glGetUniformLocation(programId, "u_Projection");

	int w = 0;
	int h = 0;

	unsigned char* data = stbi_load("./src/res/hm3.png", &w, &h, NULL, 4); // 4 = how many channels i.e. RGBA

	if (!data)
	{
		throw std::runtime_error("!data");
	}

	texture = wo::CreateTexture2Drgba(data, w, h);

	delete[] data;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);
}

texturing::~texturing()
{

}