#pragma once

#include "GL/glew.h"
#include "GLCaller.hpp"

class OpenGL
{
    inline static int s_MaxCombinedTextureImageUnits;
    inline static int s_MaxTextureImageUnits;

    inline static int s_MaxVertexUniformComponents;
    inline static int s_MaxTessControlUniformComponents;
    inline static int s_MaxTessEvaluationUniformComponents;
    inline static int s_MaxGeometryUniformComponents;
    inline static int s_MaxFragmentUniformComponents;
    inline static int s_MaxComputeUniformComponents;

    inline static int s_MaxCombinedVertexUniformComponents;
    inline static int s_MaxCombinedTessControlUniformComponents;
    inline static int s_MaxCombinedTessEvaluationUniformComponents;
    inline static int s_MaxCombinedGeometryUniformComponents;
    inline static int s_MaxCombinedFragmentUniformComponents;
    inline static int s_MaxCombinedComputeUniformComponents;

public:

    inline static const int& MaxCombinedTextureImageUnits = s_MaxCombinedTextureImageUnits;
    inline static const int& MaxTextureImageUnits = s_MaxTextureImageUnits;

    inline static const int& MaxVertexUniformComponents = s_MaxVertexUniformComponents;
    inline static const int& MaxTessControlUniformComponents = s_MaxTessControlUniformComponents;
    inline static const int& MaxTessEvaluationUniformComponents = s_MaxTessEvaluationUniformComponents;
    inline static const int& MaxGeometryUniformComponents = s_MaxGeometryUniformComponents;
    inline static const int& MaxFragmentUniformComponents = s_MaxFragmentUniformComponents;
    inline static const int& MaxComputeUniformComponents = s_MaxComputeUniformComponents;

    inline static const int& MaxCombinedVertexUniformComponents = s_MaxCombinedVertexUniformComponents;
    inline static const int& MaxCombinedTessControlUniformComponents = s_MaxCombinedTessControlUniformComponents;
    inline static const int& MaxCombinedTessEvaluationUniformComponents = s_MaxCombinedTessEvaluationUniformComponents;
    inline static const int& MaxCombinedGeometryUniformComponents = s_MaxCombinedGeometryUniformComponents;
    inline static const int& MaxCombinedFragmentUniformComponents = s_MaxCombinedFragmentUniformComponents;
    inline static const int& MaxCombinedComputeUniformComponents = s_MaxCombinedComputeUniformComponents;

    inline static void LoadParameters()
    {
        GLCall(glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &s_MaxCombinedTextureImageUnits));
        GLCall(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &s_MaxTextureImageUnits));

        GLCall(glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &s_MaxVertexUniformComponents));
        GLCall(glGetIntegerv(GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS, &s_MaxTessControlUniformComponents));
        GLCall(glGetIntegerv(GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS, &s_MaxTessEvaluationUniformComponents));
        GLCall(glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS, &s_MaxGeometryUniformComponents));
        GLCall(glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &s_MaxFragmentUniformComponents));
        GLCall(glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_COMPONENTS, &s_MaxComputeUniformComponents));

        GLCall(glGetIntegerv(GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS, &s_MaxCombinedVertexUniformComponents));
        GLCall(glGetIntegerv(GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS, &s_MaxCombinedTessControlUniformComponents));
        GLCall(glGetIntegerv(GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS, &s_MaxCombinedTessEvaluationUniformComponents));
        GLCall(glGetIntegerv(GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS, &s_MaxCombinedGeometryUniformComponents));
        GLCall(glGetIntegerv(GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS, &s_MaxCombinedFragmentUniformComponents));
        GLCall(glGetIntegerv(GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS, &s_MaxCombinedComputeUniformComponents));
    }
};