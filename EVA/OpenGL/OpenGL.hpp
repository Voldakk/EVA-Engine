#pragma once

#include "GL/glew.h"
#include "GLCaller.hpp"

#include <iostream>

#define GLINTVAL(NAME) \
private: \
inline static int s_##NAME; \
public: \
inline static const int& NAME = s_##NAME;

#define GETGLINTVAL(VAL, VAR) \
GLCall(glGetIntegerv(VAL, &s_##VAR)); \
std::cout << #VAL << " = " << s_##VAR << std::endl;

class OpenGL
{
    GLINTVAL(MaxCombinedTextureImageUnits);
    GLINTVAL(MaxTextureImageUnits);

    GLINTVAL(MaxVertexUniformComponents);
    GLINTVAL(MaxTessControlUniformComponents);
    GLINTVAL(MaxTessEvaluationUniformComponents);
    GLINTVAL(MaxGeometryUniformComponents);
    GLINTVAL(MaxFragmentUniformComponents);
    GLINTVAL(MaxComputeUniformComponents);

    GLINTVAL(MaxCombinedVertexUniformComponents);
    GLINTVAL(MaxCombinedTessControlUniformComponents);
    GLINTVAL(MaxCombinedTessEvaluationUniformComponents);
    GLINTVAL(MaxCombinedGeometryUniformComponents);
    GLINTVAL(MaxCombinedFragmentUniformComponents);
    GLINTVAL(MaxCombinedComputeUniformComponents);

public:

    inline static void LoadParameters()
    {
        GETGLINTVAL(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, MaxCombinedTextureImageUnits);
        GETGLINTVAL(GL_MAX_TEXTURE_IMAGE_UNITS, MaxTextureImageUnits);

        GETGLINTVAL(GL_MAX_VERTEX_UNIFORM_COMPONENTS, MaxVertexUniformComponents);
        GETGLINTVAL(GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS, MaxTessControlUniformComponents);
        GETGLINTVAL(GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS, MaxTessEvaluationUniformComponents);
        GETGLINTVAL(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS, MaxGeometryUniformComponents);
        GETGLINTVAL(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, MaxFragmentUniformComponents);
        GETGLINTVAL(GL_MAX_COMPUTE_UNIFORM_COMPONENTS, MaxComputeUniformComponents);

        GETGLINTVAL(GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS, MaxCombinedVertexUniformComponents);
        GETGLINTVAL(GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS, MaxCombinedTessControlUniformComponents);
        GETGLINTVAL(GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS, MaxCombinedTessEvaluationUniformComponents);
        GETGLINTVAL(GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS, MaxCombinedGeometryUniformComponents);
        GETGLINTVAL(GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS, MaxCombinedFragmentUniformComponents);
        GETGLINTVAL(GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS, MaxCombinedComputeUniformComponents);
    }
};