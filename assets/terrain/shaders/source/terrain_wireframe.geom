#version 330
#define MAX_MATERIALS 3

layout (triangles) in;
layout (line_strip, max_vertices=4) out;

in vec2 uvGeo[];

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

uniform mat4 viewProjection;
uniform sampler2D normalmap;
uniform sampler2D splatmap;
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
			
			vec3 normal = normalize(texture(normalmap, uvGeo[k]).rbg);
			
			vec4 blendValues = texture(splatmap, uvGeo[k]).rgba;
			float[4] blendValuesArray = float[](blendValues.r, blendValues.g, blendValues.b, blendValues.a);
			
			float scale = 0;
			for (int i = 0; i < 3; i++)
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
		EmitVertex();
	}

	vec4 worldPos = gl_in[0].gl_Position + vec4(displacement[0], 0);
	gl_Position = viewProjection * worldPos;
	EmitVertex();
	
	EndPrimitive();
}