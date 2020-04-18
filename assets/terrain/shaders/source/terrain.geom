#version 430
#define MAX_MATERIALS 8
#define MAX_LIGHTS 10

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

in vec2 uvGeo[];

out vec2 uvFrag;
out vec3 posFrag;
out vec3 tangentFrag;

// Materials
uniform int numMaterials;
uniform struct Material
{
	vec4 tint;
	vec2 tiling;
	float heightScale;

    sampler2D albedoMap;
    sampler2D metRouAoMap;
    sampler2D normalHeightMap;
} materials[MAX_MATERIALS];

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

in vec4 posLightSpaceGeo [3][MAX_LIGHTS];
out vec4 posLightSpaceFrag [MAX_LIGHTS];

uniform mat4 viewProjection;
uniform sampler2D normalmap;
uniform sampler2D splatmap0;
uniform sampler2D splatmap1;
uniform vec3 cameraPosition;
uniform int tbnRange;

vec3 tangent;
vec3 displacement[3];

void calcTangent()
{	
	vec3 v0 = gl_in[0].gl_Position.xyz;
	vec3 v1 = gl_in[1].gl_Position.xyz;
	vec3 v2 = gl_in[2].gl_Position.xyz;

	// edges of the face/triangle
    vec3 e1 = v1 - v0;
    vec3 e2 = v2 - v0;
	
	vec2 uv0 = uvGeo[0];
	vec2 uv1 = uvGeo[1];
	vec2 uv2 = uvGeo[2];

    vec2 deltaUV1 = uv1 - uv0;
	vec2 deltaUV2 = uv2 - uv0;
	
	float r = 1.0 / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
	
	tangent = normalize((e1 * deltaUV2.y - e2 * deltaUV1.y)*r);
}

void main() {

	for (int i = 0; i < 3; ++i)
	{
		displacement[i] = vec3(0);
	}

	float dist = (distance(gl_in[0].gl_Position.xyz, cameraPosition) + distance(gl_in[1].gl_Position.xyz, cameraPosition) + distance(gl_in[2].gl_Position.xyz, cameraPosition)) / 3;
	
	if (dist < tbnRange){
	
		calcTangent();
		
		for(int k=0; k<gl_in.length(); k++){ 
			
			displacement[k] = vec3(0,1,0);
			
			float height = gl_in[k].gl_Position.y;
			
			vec4 blendValues0 = texture(splatmap0, uvGeo[k]).rgba;
			vec4 blendValues1 = texture(splatmap1, uvGeo[k]).rgba;
			float[MAX_MATERIALS] blendValuesArray = float[](blendValues0.r, blendValues0.g, blendValues0.b, blendValues0.a, blendValues1.r, blendValues1.g, blendValues1.b, blendValues1.a);
			
			float scale = 0;
			for (int i = 0; i < numMaterials; i++)
			{
				scale += texture(materials[i].normalHeightMap, uvGeo[k]
							* materials[i].tiling).a 
							* materials[i].heightScale 
							* blendValuesArray[i];
			}
						
			float attenuation = clamp(-distance(gl_in[k].gl_Position.xyz, cameraPosition) / (tbnRange - 50) + 1, 0.0, 1.0);
			scale *= attenuation;

			displacement[k] *= scale;
		}	
	}
	
	for (int i = 0; i < gl_in.length(); ++i)
	{
		vec4 worldPos = gl_in[i].gl_Position + vec4(displacement[i], 0);
		gl_Position = viewProjection * worldPos;
		uvFrag = uvGeo[i];
		posFrag = (worldPos).xyz;
		tangentFrag = tangent;
		posLightSpaceFrag = posLightSpaceGeo[i];
		EmitVertex();
	}
	
	EndPrimitive();
}