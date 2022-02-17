#include <GL/glew.h>
#include<stdexcept>

#include "../openglReq.h"

namespace wo
{
	GLuint CreateVBO(GLsizeiptr _size, const GLvoid* _data, GLenum _usage, GLenum _target)
	{
		// Generate new buffer
		GLuint id = 0;

		// Create a new VBO on the GPU and bind it
		glGenBuffers(1, &id);

		if (!id)
		{
			throw std::runtime_error("VBO Gen failed");
		}

		glBindBuffer(_target, id);

		// Upload a copy of the data from memory into the new VBO
		glBufferData(_target, _size, _data, _usage);

		// Reset the state
		glBindBuffer(_target, 0);

		return id;
	}

	GLuint CompileShader(GLenum _target, const GLchar* _shaderSRC)
	{
		GLuint id = glCreateShader(_target);
		glShaderSource(id, 1, &_shaderSRC, NULL);
		glCompileShader(id);
		GLint success = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			throw std::runtime_error("!success (vertex)");
		}
		return id;
	}

	GLuint CreateTexture2Drgba(const void* _data, GLsizei _width, GLsizei _height)
	{
		GLuint id = 0;
		// Generate
		glGenTextures(1, &id);
		if (!id) { throw std::runtime_error("Texture gen failed"); }
		// Bind
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data);
		// Generate mipmap for texture
		glGenerateMipmap(GL_TEXTURE_2D);
		// Unbind
		glBindTexture(GL_TEXTURE_2D, 0);
		return id;
	}
}
