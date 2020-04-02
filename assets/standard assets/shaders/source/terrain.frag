#version 150 core

struct Material
{
    sampler2D diffusemap;
    sampler2D normalmap;
    sampler2D heightmap;
    float heightScale;
    vec2 tiling;
};

in vec2 uvFrag;
in vec3 posFrag;
in vec3 tangentFrag;

out vec4 outputColor;

uniform sampler2D normalmap;
uniform sampler2D splatmap;
uniform vec3 cameraPosition;
uniform Material materials[3];
uniform int tbnRange;


const vec3 direction = vec3(0.1, -1, 0.1);
const float intensity = 1.2;
const vec3 color = vec3(0.1, 1, 0.1);

float diffuse(vec3 direction, vec3 normal, float intensity)
{
	return max(0.04, dot(normal, -direction) * intensity);
}

void main()
{
	float dist = length(cameraPosition - posFrag);
	float height = posFrag.y;

	vec3 normal = normalize(texture(normalmap, uvFrag).rbg);
	
	vec4 blendValues = texture(splatmap, uvFrag).rgba;
	float[4] blendValuesArray = float[](blendValues.r, blendValues.g, blendValues.b, blendValues.a);
	
	if (dist < tbnRange - 50)
	{
		float attenuation = clamp(-dist / (tbnRange - 50) + 1, 0.0, 1.0);
		
		vec3 bitangent = normalize(cross(tangentFrag, normal));
		mat3 TBN = mat3(bitangent,normal,tangentFrag);
		
		vec3 bumpNormal;
		for (int i = 0; i < 3; i++)
		{			
			bumpNormal += (2*(texture(materials[i].normalmap, uvFrag * materials[i].tiling).rbg) - 1) * blendValuesArray[i];
		}
		
		bumpNormal = normalize(bumpNormal);
		
		bumpNormal.xz *= attenuation;
		
		normal = normalize(TBN * bumpNormal);
	}
	
	vec3 fragColor = vec3(0);
	for(int i = 0; i < 3; ++i)
	{
		fragColor += texture(materials[i].diffusemap, uvFrag * materials[i].tiling).rgb * blendValuesArray[i];
	}
	
	float diffuse = diffuse(direction, normal, intensity);

	fragColor *= diffuse;
	
	outputColor = vec4(fragColor, 1.0);
}