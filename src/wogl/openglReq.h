#ifndef _OPENGLREQ_H_
#define _OPENGLREQ_H_

#include<GL/glew.h>

namespace wo
{
	/// @brief Creates VBO
	/// @param _size Size and bites of buffer data 
	/// @param _data Pointer to the data
	/// @param _usage Expected usage of data
	/// @param _target Target buffer object
	/// @return Handle and id of new VBO
	GLuint CreateVBO(GLsizeiptr _size, const GLvoid* _data, GLenum _usage = GL_DYNAMIC_DRAW, GLenum _target = GL_ARRAY_BUFFER);

	/// @brief Compile 1 whole shader
	/// @param _target Type of shader
	/// @param shaderSRC Link to shader text
	/// @return Compiled shader
	GLuint CompileShader(GLenum _target, const GLchar* _shaderSRC);

	/// @brief Create a 2D texture in rgba format
	/// @param _data Pixel data to be processed
	/// @param _width of texture
	/// @param _height of texture
	/// @return handle for texture
	GLuint CreateTexture2Drgba(const void* _data, GLsizei _width, GLsizei _height);
}
#endif // !_OPENGLREQ_H_