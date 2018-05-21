#version 430

layout (vertices = 16) out;

const int AB = 2;
const int BC = 3;
const int CD = 0;
const int DA = 1;

uniform int tessLevelOuter;
uniform int tessLevelInner0;
uniform int tessLevelInner1;

void main()
{
	if(gl_InvocationID == 0)
	{
		gl_TessLevelOuter[AB] = tessLevelOuter;
		gl_TessLevelOuter[BC] = tessLevelOuter;
		gl_TessLevelOuter[CD] = tessLevelOuter;
		gl_TessLevelOuter[DA] = tessLevelOuter;

		gl_TessLevelInner[0] = tessLevelInner0;
		gl_TessLevelInner[1] = tessLevelInner1;
	}

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}