#version 150

uniform samplerCube skytexture;

in vec3 fragTexCoord;
in vec3 fragPos;

out vec4 fragColor;

uniform mat4 viewProjection;
uniform vec3 skyTint;
uniform vec3 sunColor;
uniform float sunSize;

// Lights
#define MAX_LIGHTS 10
uniform int numLights;
uniform struct Light
{
   vec4 position;
   vec3 color;
   float attenuation;
   float ambientCoefficient;
   sampler2D shadowMap;
   mat4 lightSpaceMatrix;
   float farPlane;

} allLights[MAX_LIGHTS];

void main()
{
	vec3 color = texture(skytexture, fragTexCoord).xyz * skyTint; 
    if (numLights > 0 && allLights[0].position.w != 1.0)
    {
        float deg = acos(dot(normalize(allLights[0].position.xyz), normalize(fragPos.xyz)));
        float a = 1 - clamp(deg, 0, sunSize) / (sunSize);
        color = mix(color, sunColor, a*a);
    }

    fragColor = vec4(color, 1.0);
}
