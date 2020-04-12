#version 330 core

uniform mat4 model;
uniform mat4 viewProjection;

layout (location = 0) in vec3 vert;

out vec3 fragTexCoord;
out vec3 fragPos;

void main()
{
	// Pass some variables to the fragment shader
    fragTexCoord = vert;
    fragPos = vert;

    // Apply all matrix transformations to vert
    gl_Position = viewProjection * model * vec4(vert, 1);
}
