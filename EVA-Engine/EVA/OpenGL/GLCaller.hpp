#pragma once

#define ASSERT(glFunc) if (!(glFunc)) __debugbreak();

#ifdef _DEBUG

#define GLCall(glFunc) GLClearError();\
    glFunc;\
    ASSERT(GLLogCall(#glFunc, __FILE__, __LINE__))

#else

#define GLCall(glFunc) glFunc;

#endif

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
