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

// Camera
uniform vec3 cameraPosition;

uniform int tessLevelOuter;
uniform int tessLevelInner0;
uniform int tessLevelInner1;

uniform int maxLod;

float Distance(vec3 point)
{
	vec4 minP = gl_in[southEast].gl_Position;
	vec4 maxP = gl_in[northWest].gl_Position;

	float dx = max(max(minP.x - point.x, point.x - maxP.x), 0);
	float dy = max(max(minP.y - point.y, point.y - maxP.y), 0);
	float dz = max(max(minP.z - point.z, point.z - maxP.z), 0);

	return sqrt(dx * dx + dy * dy + dz * dz);
}

void main()
{
	if(gl_InvocationID == 0)
	{
		float dist = Distance(cameraPosition);

		int lod = int(max(1, maxLod - abs(dist / 10)));

		gl_TessLevelOuter[north] = tessLevelOuter;
		gl_TessLevelOuter[west] = tessLevelOuter;
		gl_TessLevelOuter[south] = tessLevelOuter;
		gl_TessLevelOuter[east] = tessLevelOuter;

		gl_TessLevelInner[0] = lod;
		gl_TessLevelInner[1] = lod;
	}

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}