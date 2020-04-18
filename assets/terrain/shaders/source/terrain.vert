#version 430

in vec2 vertPos;
in mat4 localMatrix;
in float tScaleNegX;
in float tScalePosX;
in float tScaleNegY;
in float tScalePosY;

out vec2 uvTC;
out mat4 mvTC;

out float tScaleNegXTC;
out float tScalePosXTC;
out float tScaleNegYTC;
out float tScalePosYTC;

uniform mat4 worldMatrix;
uniform sampler2D heightmap;
uniform mat4 view;

void main()
{
	vec3 localPosition = (localMatrix * vec4(vertPos.x, 0, vertPos.y, 1)).xyz;
	uvTC = localPosition.xz;
	mvTC = view * worldMatrix;

	tScaleNegXTC = tScaleNegX;
	tScalePosXTC = tScalePosX;
	tScaleNegYTC = tScaleNegY;
	tScalePosYTC = tScalePosY;

	float height = texture(heightmap, uvTC).r;
	localPosition.y = height;

	gl_Position = worldMatrix * vec4(localPosition, 1);
}