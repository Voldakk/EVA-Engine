#version 430

layout (vertices = 4) out;

const int north = 3;
const int west  = 2;
const int south = 1; 
const int east  = 0;

const int northWest = 3; 
const int northEast = 2;
const int southWest = 1;
const int southEast = 0;

uniform int maxLod;

in float[] vertLod;
in float[][4] vertLodSides;

void main()
{
	if(gl_InvocationID == 0)
	{
		float tessLevel = 1 + (maxLod - vertLod[0]);

		float tess = pow(2, vertLod[0]);

		gl_TessLevelOuter[north] = 1;
		gl_TessLevelOuter[west] = 1;
		gl_TessLevelOuter[south] = 1;
		gl_TessLevelOuter[east] = 1;

		gl_TessLevelInner[0] = 1;
		gl_TessLevelInner[1] = 1;
	}

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}