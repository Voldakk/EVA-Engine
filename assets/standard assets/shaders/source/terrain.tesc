#version 430

layout(vertices = 16) out;

const int AB = 2;
const int BC = 3;
const int CD = 0;
const int DA = 1;

uniform float tessFactor;
uniform float tessSlope;
uniform float tessShift;

uniform vec3 cameraPosition;

float lodFactor(float dist) 
{
	return max(0.0, tessFactor/pow(dist, tessSlope) + tessShift);
}
		
void main()
{
	if(gl_InvocationID == 0)
	{
		vec3 ab = vec3(gl_in[0].gl_Position + gl_in[3].gl_Position) / 2.0;	
		vec3 bc = vec3(gl_in[3].gl_Position + gl_in[15].gl_Position) / 2.0;
		vec3 cd = vec3(gl_in[15].gl_Position + gl_in[12].gl_Position) / 2.0;
		vec3 da = vec3(gl_in[12].gl_Position + gl_in[0].gl_Position) / 2.0;		
		
		float distAB = distance(ab, cameraPosition);
		float distBC = distance(bc, cameraPosition);
		float distCD = distance(cd, cameraPosition);
		float distDA = distance(da, cameraPosition);
		
		gl_TessLevelOuter[AB] = mix(1, gl_MaxTessGenLevel, lodFactor(distAB));
		gl_TessLevelOuter[BC] = mix(1, gl_MaxTessGenLevel, lodFactor(distBC));
		gl_TessLevelOuter[CD] = mix(1, gl_MaxTessGenLevel, lodFactor(distCD));
		gl_TessLevelOuter[DA] = mix(1, gl_MaxTessGenLevel, lodFactor(distDA));
		
		gl_TessLevelInner[0] = (gl_TessLevelOuter[BC] + gl_TessLevelOuter[DA])/4;
		gl_TessLevelInner[1] = (gl_TessLevelOuter[AB] + gl_TessLevelOuter[CD])/4;
	}
	
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
