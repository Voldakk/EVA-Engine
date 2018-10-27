#version 430

layout (location = 0) in vec3 vertPos;
layout (location = 1) in mat4 instanceMatrix;
layout (location = 5) in float lod;
layout (location = 6) in vec4 lodSides;

const int north = 3;
const int west  = 2;
const int south = 1; 
const int east  = 0;

out float vertLod;
out float[4] vertLodSides;

void main()
{
	gl_Position = instanceMatrix * vec4(vertPos, 1);
	
	vertLod = lod;

	vertLodSides[north] = lodSides.x;
	vertLodSides[west] = lodSides.y;
	vertLodSides[south] = lodSides.z;
	vertLodSides[east] = lodSides.w;
}