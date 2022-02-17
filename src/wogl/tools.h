#ifndef _TOOLS_H_
#define _TOOLS_H_

// External Dependencies
#include <SDL2/SDL.h>
#include <GL/glew.h>

// STL (Standard Template Library) Dependencies
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>

namespace wo
{
	/// @brief Reads file contents
	/// @param _filepath Filepath to the file
	/// @return Extracted contents
	std::string ReadFile(const char* _filepath);
}

#endif // !_TOOLS_H_