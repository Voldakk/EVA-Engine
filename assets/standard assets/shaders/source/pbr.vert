#version 330 core
layout (location = 0) in vec3 vertPosition;
layout (location = 1) in vec3 vertNormal;
layout (location = 2) in vec2 vertTexCoord;
layout (location = 3) in vec3 vertTangent;
layout (location = 4) in vec3 vertBitangent;  

out vec2 fragTexCoord;
out vec3 fragPos;
out vec3 fragNormal;

uniform mat4 viewProjection;
uniform mat4 model;

void main()
{
    fragTexCoord = vertTexCoord;
    fragNormal = mat3(model) * vertNormal;
    fragPos = vec3(model * vec4(vertPosition, 1.0));

    gl_Position =  viewProjection * vec4(fragPos, 1.0);
}