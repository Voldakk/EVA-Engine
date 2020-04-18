#version 430

layout(vertices = 4) out;

in vec2 uvTC[];
in mat4 mvTC[];

in float tScaleNegXTC[];
in float tScalePosXTC[];
in float tScaleNegYTC[];
in float tScalePosYTC[];

out vec2 uvTE[];

uniform vec3 cameraPosition;

uniform sampler2D heightMap;
uniform float scaleY;

//gl_TessLevelOuter[0] North
//gl_TessLevelOuter[1] East
//gl_TessLevelOuter[2] South 
//gl_TessLevelOuter[3] West
		
void main(void)
{
	if(gl_InvocationID == 0)
	{
		// Outer tessellation level
		gl_TessLevelOuter[0] = gl_MaxTessGenLevel;
		gl_TessLevelOuter[1] = gl_MaxTessGenLevel;
		gl_TessLevelOuter[2] = gl_MaxTessGenLevel;
		gl_TessLevelOuter[3] = gl_MaxTessGenLevel;
	
		gl_TessLevelOuter[0] = max(2.0, gl_TessLevelOuter[0] / tScalePosYTC[0]);
		gl_TessLevelOuter[1] = max(2.0, gl_TessLevelOuter[1] / tScalePosXTC[0]);
		gl_TessLevelOuter[2] = max(2.0, gl_TessLevelOuter[2] / tScaleNegYTC[0]);
		gl_TessLevelOuter[3] = max(2.0, gl_TessLevelOuter[3] / tScaleNegXTC[0]);

		// Inner tessellation level
		gl_TessLevelInner[0] = 0.5 * (gl_TessLevelOuter[1] + gl_TessLevelOuter[3]);
		gl_TessLevelInner[1] = 0.5 * (gl_TessLevelOuter[0] + gl_TessLevelOuter[2]);
	}

	// Pass the patch verts along
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

	// Output heightmap coordinates
	uvTE[gl_InvocationID] = uvTC[gl_InvocationID];
}
