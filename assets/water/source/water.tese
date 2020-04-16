#version 430
#define MAX_OCTAVES 10

layout(quads, fractional_even_spacing, cw) in;

flat in vec2 scaleTE[];
out vec3 normalGeo;

uniform float time;

// Water
uniform float scaleY;
uniform vec2 waveSpeed;

// Noise
uniform float scale;
uniform int numOctaves;
uniform vec2[MAX_OCTAVES] octaveOffsets;
uniform float persistance;
uniform float lacunarity;
uniform float maxPossibleHeight;

// Simplex 2D noise
vec3 permute(vec3 x) 
{ 
	return mod(((x*34.0)+1.0)*x, 289.0); 
}

float snoise(vec2 v)
{
	const vec4 C = vec4(0.211324865405187, 0.366025403784439, -0.577350269189626, 0.024390243902439);
	
	vec2 i = floor(v + dot(v, C.yy));
	vec2 x0 = v - i + dot(i, C.xx);
	vec2 i1;
	i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
	
	vec4 x12 = x0.xyxy + C.xxzz;
	x12.xy -= i1;
	
	i = mod(i, 289.0);
	
	vec3 p = permute(permute(i.y + vec3(0.0, i1.y, 1.0)) + i.x + vec3(0.0, i1.x, 1.0));
	
	vec3 m = max(0.5 - vec3(dot(x0, x0), dot(x12.xy, x12.xy), dot(x12.zw, x12.zw)), 0.0);
	m = m * m;
	m = m * m;
	
	vec3 x = 2.0 * fract(p * C.www) - 1.0;
	vec3 h = abs(x) - 0.5;
	vec3 ox = floor(x + 0.5);
	vec3 a0 = x - ox;
	m *= 1.79284291400159 - 0.85373472095314 * (a0 * a0 + h * h);
	
	vec3 g;
	g.x = a0.x * x0.x + h.x * x0.y;
	g.yz = a0.yz * x12.xz + h.yz * x12.yw;
	
	return 130.0 * dot(m, g);
}

// Octaves
float GenerateNoise(vec2 position)
{
    float amplitude = 1;
    float frequency = 1;
    float noiseHeight = 0;

    for (int i = 0; i < numOctaves; ++i)
    {
        float sampleX = (octaveOffsets[i].x + position.x) / scale * frequency;
        float sampleY = (octaveOffsets[i].y + position.y) / scale * frequency;

        float value = snoise(vec2(sampleX, sampleY)) * 2 - 1;
        noiseHeight += value * amplitude;

        amplitude *= persistance;
        frequency *= lacunarity;
    }

    //float normalizedHeight = (noiseHeight + 1) / (maxPossibleHeight / 0.9f);
    return noiseHeight;
}

// ----------------------------------------------------------------------------
void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
	
	// world position
	vec4 position =
	((1 - u) * (1 - v) * gl_in[2].gl_Position +
	    u    * (1 - v) * gl_in[0].gl_Position +
	    u    *    v    * gl_in[1].gl_Position +
	 (1 - u) *    v    * gl_in[3].gl_Position);
    
	vec2 offset = waveSpeed * time;

	vec3 tangent = position.xyz + vec3(scaleTE[0].x, 0.0, 0.0);
	vec3 bitangent = position.xyz + vec3(0.0, 0.0, scaleTE[0].y);

	position.y += GenerateNoise(position.xz + offset) * scaleY;
	tangent.y += GenerateNoise(tangent.xz + offset) * scaleY;
	bitangent.y += GenerateNoise(bitangent.xz + offset) * scaleY;

	normalGeo = cross(tangent - position.xyz, bitangent - position.xyz);
	normalGeo = normalize(normalGeo);

	gl_Position = position;
}
