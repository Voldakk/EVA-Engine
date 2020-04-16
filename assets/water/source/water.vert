#version 430

in vec2 vertPos;
in mat4 worldMatrix;
in mat4 localMatrix;
in float tScaleNegX;
in float tScalePosX;
in float tScaleNegY;
in float tScalePosY;

out float tScaleNegXTC;
out float tScalePosXTC;
out float tScaleNegYTC;
out float tScalePosYTC;

flat out vec2 scaleTC;

void main()
{
	vec3 localPosition = (localMatrix * vec4(vertPos.x, 0, vertPos.y, 1)).xyz;

	tScaleNegXTC = tScaleNegX;
	tScalePosXTC = tScalePosX;
	tScaleNegYTC = tScaleNegY;
	tScalePosYTC = tScalePosY;

	scaleTC.x = localMatrix[0][0] * worldMatrix[0][0]; 
	scaleTC.y = localMatrix[3][3] * worldMatrix[3][3]; 

	gl_Position = worldMatrix * vec4(localPosition, 1);
}
