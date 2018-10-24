#include "EVA/OpenGL.hpp"

#include <iostream>

void GLClearError() { while (glGetError() != GL_NO_ERROR); }
bool GLLogCall(const char* function, const char* file, const int line)
{
	while (const auto error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ");\n" << function << " in " << file << "\nat line: " << line << std::endl;
		std::cin.get();
		return false;
	}
	return true;
}
