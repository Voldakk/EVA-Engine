#version 150 core

in vec2 uvFrag;

out vec4 outputColor;

uniform sampler2D normalmap;

const vec3 direction = vec3(0.1, -1, 0.1);
const float intensity = 1.2;
const vec3 color = vec3(0.1, 1, 0.1);

float diffuse(vec3 direction, vec3 normal, float intensity)
{
	return max(0.01, dot(normal, -direction) * intensity);
}

void main()
{
	vec3 normal = texture(normalmap, uvFrag).rgb;

	float diffuse = diffuse(direction, normal, intensity);

	outputColor = vec4(color * diffuse, 1.0);
}