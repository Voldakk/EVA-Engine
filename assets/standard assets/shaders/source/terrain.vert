#version 430

in vec2 vertPos;
in mat4 worldMatrix;

void main()
{
	gl_Position = worldMatrix * vec4(vertPos.x, 0, vertPos.y, 1);
}