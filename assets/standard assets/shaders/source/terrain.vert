#version 430

in vec2 vertPos;
in mat4 instanceMatrix;
in float lod;
in vec2 index;

void main()
{
	gl_Position = instanceMatrix * vec4(vertPos.x, 0, vertPos.y, 1);
}