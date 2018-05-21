#version 430

layout (location = 0) in vec3 vertPos;
layout (location = 1) in mat4 instanceMatrix;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool instancing;

void main()
{
	mat4 modelMatrix = model;

	if(instancing)
		modelMatrix = instanceMatrix;

	gl_Position = projection * view * modelMatrix * vec4(vertPos, 1);
}