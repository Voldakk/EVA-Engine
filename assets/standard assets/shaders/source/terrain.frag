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
uniform vec3 cameraPosition;
uniform Material materials[2];
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

	vec3 normal = normalize(texture(normalmap, uvFrag).rgb);

    vec3 material0Color = texture(materials[0].diffusemap, uvFrag * materials[0].tiling).rgb;
    vec3 material1Color = texture(materials[1].diffusemap, uvFrag * materials[1].tiling).rgb;

    float[2] materialAlpha = float[](0, 0);
    if(normal.y > 0.5)
    {
        materialAlpha[1] = 1;
    }
    else
    {
        materialAlpha[0] = 1;
    }

    if(dist < tbnRange - 50)
    {
        float attenuation = clamp(dist / (tbnRange - 50) + 1, 0.0, 1.0);

        vec3 bitangent = normalize(cross(tangentFrag, normal));
        mat3 tbn = mat3(bitangent, normal, tangentFrag);

        vec3 bumpNormal;
        for (int i = 0; i < 2; i++)
        {
            bumpNormal += (2 * (texture(materials[i].normalmap, uvFrag * materials[i].tiling).rbg) - 1) * materialAlpha[i];
        }

        bumpNormal = normalize(bumpNormal);

        bumpNormal.xz *= attenuation;

        normal = normalize(tbn * bumpNormal);
    }

    vec3 fragColor = material0Color * materialAlpha[0] + material1Color * materialAlpha[1];

	float diff = diffuse(direction, normal, intensity);

    fragColor *= diff;

	outputColor = vec4(fragColor, 1.0);
}
