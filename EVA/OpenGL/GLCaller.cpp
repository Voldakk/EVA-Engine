#include "EVA/OpenGL.hpp"

#include <iostream>

void GLClearError() { while (glGetError() != GL_NO_ERROR); }
bool GLErrorLogCall(const char* function, const char* file, const int line)
{
	while (const auto error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ");\n" << function << std::endl << file << " (" << line << ")" << std::endl;
		std::cin.get();
		return false;
	}
	return true;
}

void GLLogCall(const char* function, const char* file, const int line)
{
	std::cout << "[OpenGL call] " << function << std::endl << file << " (" << line << ")" << std::endl;
}
