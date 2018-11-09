#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool instancing;

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertNormal;
layout (location = 2) in vec2 vertTexCoord;
layout (location = 5) in mat4 instanceMatrix;

out vec3 geomPos;
out vec3 geomNormal;
out vec2 geomTexCoord;

void main() 
{
	mat4 modelMatrix = model;

	if(instancing)
		modelMatrix = instanceMatrix;

    // Pass some variables to the fragment shader
	geomPos = vec3(modelMatrix * vec4(vertPos, 1));
	geomNormal = vertNormal;
    geomTexCoord = vertTexCoord;

    // Apply all matrix transformations to vert
    gl_Position = projection * view * modelMatrix * vec4(vertPos, 1);
}