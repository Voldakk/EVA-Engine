#version 430
#define MAX_LIGHTS 10

layout(quads, fractional_even_spacing, cw) in;

in vec2 uvTese[];

out vec2 uvGeo;

uniform sampler2D heightmap;
uniform float scaleY;

// Lights
uniform int numLights;
uniform struct Light
{
   vec4 position;
   vec3 color;
   float farPlane;
   int hasShadows;
   mat4 lightSpaceMatrix;
   sampler2D shadowMap;
   samplerCube shadowCubeMap;
} allLights[MAX_LIGHTS];

out vec4 posLightSpaceGeo [MAX_LIGHTS];

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

	uvGeo =
	((1 - u) * (1 - v) * uvTese[2] +
	    u    * (1 - v) * uvTese[0] +
	    u    *    v    * uvTese[1] +
	 (1 - u) *    v    * uvTese[3]);
	
	float height = texture(heightmap, uvGeo).r;

	position.y = height * scaleY;

	gl_Position = position;

	for(int i = 0; i < numLights; ++i)
    {
        posLightSpaceGeo[i] = allLights[i].lightSpaceMatrix * position;
    }
}
