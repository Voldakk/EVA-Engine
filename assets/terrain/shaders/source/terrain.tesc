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

// Dynamic level of detail using camera distance algorithm.
float dlodCameraDistance(vec4 p0, vec4 p1, vec2 t0, vec2 t1)
{
	vec4 samp = texture(heightMap, t0);
	p0.y = samp[0] * scaleY;
	samp = texture(heightMap, t1);
	p1.y = samp[0] * scaleY;

	vec4 view0 = mvTC[0] * p0;
	vec4 view1 = mvTC[0] * p1;

	float MinDepth = 10.0;
	float MaxDepth = 10000.0;

	float d0 = clamp( (abs(p0.z) - MinDepth) / (MaxDepth - MinDepth), 0.0, 1.0);
	float d1 = clamp( (abs(p1.z) - MinDepth) / (MaxDepth - MinDepth), 0.0, 1.0);

	float t = mix(64, 2, (d0 + d1) * 0.5);

	if (t <= 2.0)
	{ 
		return 2.0;
	}
	if (t <= 4.0)
	{ 
		return 4.0;
	}
	if (t <= 8.0)
	{ 
		return 8.0;
	}
	if (t <= 16.0)
	{ 
		return 16.0;
	}
	if (t <= 32.0)
	{ 
		return 32.0;
	}
	
	return 64.0;
}
		
void main(void)
{
	if(gl_InvocationID == 0)
	{
		// Outer tessellation level
		gl_TessLevelOuter[0] = gl_MaxTessGenLevel;// dlodCameraDistance(gl_in[3].gl_Position, gl_in[0].gl_Position, uvTC[3], uvTC[0]);
		gl_TessLevelOuter[1] = gl_MaxTessGenLevel;// dlodCameraDistance(gl_in[0].gl_Position, gl_in[1].gl_Position, uvTC[0], uvTC[1]);
		gl_TessLevelOuter[2] = gl_MaxTessGenLevel;// dlodCameraDistance(gl_in[1].gl_Position, gl_in[2].gl_Position, uvTC[1], uvTC[2]);
		gl_TessLevelOuter[3] = gl_MaxTessGenLevel;// dlodCameraDistance(gl_in[2].gl_Position, gl_in[3].gl_Position, uvTC[2], uvTC[3]);
	
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
