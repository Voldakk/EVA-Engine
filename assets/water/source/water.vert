#version 430

in vec2 vertPos;
in mat4 localMatrix;
in float tScaleNegX;
in float tScalePosX;
in float tScaleNegY;
in float tScalePosY;

out float tScaleNegXTC;
out float tScalePosXTC;
out float tScaleNegYTC;
out float tScalePosYTC;

uniform mat4 worldMatrix;

void main()
{
	vec3 localPosition = (localMatrix * vec4(vertPos.x, 0, vertPos.y, 1)).xyz;

	tScaleNegXTC = tScaleNegX;
	tScalePosXTC = tScalePosX;
	tScaleNegYTC = tScaleNegY;
	tScalePosYTC = tScalePosY;

	gl_Position = worldMatrix * vec4(localPosition, 1);
}
