#pragma once

//#define LOG_GL_CALLS

#define ASSERT(glFunc) if (!(glFunc)) __debugbreak();

#ifdef _DEBUG
#ifdef LOG_GL_CALLS

#define GLCall(glFunc) GLClearError();\
    GLLogCall(#glFunc, __FILE__, __LINE__);\
    glFunc;\
    ASSERT(GLErrorLogCall(#glFunc, __FILE__, __LINE__))
#else

#define GLCall(glFunc) GLClearError();\
    glFunc;\
    ASSERT(GLErrorLogCall(#glFunc, __FILE__, __LINE__))

#endif //LOG_GL_CALLS
#else

#define GLCall(glFunc) glFunc;

#endif //_DEBUG

void GLClearError();
bool GLErrorLogCall(const char* function, const char* file, int line);
void GLLogCall(const char* function, const char* file, int line);
