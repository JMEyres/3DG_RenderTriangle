#include "../tools.h"

namespace wo
{
	std::string ReadFile(const char* _filepath)
	{
		// Open input file from filepath
		std::ifstream file(_filepath);
		if (!file.is_open()) { throw std::runtime_error("Couldn't open file"); }

		// Retrieve file contents
		std::string data, ln;
		while (std::getline(file, ln)) { data += ln; }
		file.close();
		return data;

	}
}