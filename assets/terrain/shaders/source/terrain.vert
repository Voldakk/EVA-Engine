#version 430

in vec2 vertPos;
in mat4 worldMatrix;
in mat4 localMatrix;

out vec2 uvTC;

uniform sampler2D heightmap;

void main()
{
	vec3 localPosition = (localMatrix * vec4(vertPos.x, 0, vertPos.y, 1)).xyz;
	uvTC = localPosition.xz;
	uvTC.x = 1 - uvTC.x; // flip x

	float height = texture(heightmap, uvTC).r;
	localPosition.y = height;

	gl_Position = worldMatrix * vec4(localPosition, 1);
}