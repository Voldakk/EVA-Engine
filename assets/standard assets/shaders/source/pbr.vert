#version 330 core

layout (location = 0) in vec3 vertPosition;
layout (location = 1) in vec3 vertNormal;
layout (location = 2) in vec2 vertUV;
layout (location = 3) in vec3 vertTangent;
layout (location = 4) in vec3 vertBitangent;  

out vec3 fragPos;
out vec2 fragUV;
out mat3 fragTBN;

uniform mat4 model;
uniform mat4 viewProjection;

// Lights
#define MAX_LIGHTS 10
uniform int numLights;
uniform struct Light
{
   vec4 position;
   vec3 color;
   float farPlane;
   int hasShadows;
   mat4 lightSpaceMatrix;
   sampler2D shadowMap;
   samplerCube shadowCubeMap;
} allLights[MAX_LIGHTS];

out vec4 allFragPosLightSpace [MAX_LIGHTS];

void main()
{
    fragPos = vec3(model * vec4(vertPosition, 1.0));
    fragUV = vertUV;

    for(int i = 0; i < numLights; ++i)
    {
        allFragPosLightSpace[i] = allLights[i].lightSpaceMatrix * vec4(fragPos, 1.0);
    }

    gl_Position =  viewProjection * model * vec4(vertPosition, 1.0);

    // Calculate TBN matrix
	vec3 T = normalize(vec3(model * vec4(vertTangent,   0.0)));
	vec3 B = normalize(vec3(model * vec4(vertBitangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(vertNormal,    0.0)));
	fragTBN = mat3(T, B, N);
}