#version 150 core

// Material settings
uniform struct Material
{
   vec4 tint_diffuse;

} material;

// In
in vec3 fragWireframeDist;
in vec3 fragVert;
in vec2 fragTexCoord;
in vec3 fragNormal;

// Out color
out vec4 finalColor;

void main()
{
    vec3 d = fwidth(fragWireframeDist);
 
    vec3 a3 = smoothstep(vec3(0.0), d * 1.5, fragWireframeDist);
    float edgeFactor = min(min(a3.x, a3.y), a3.z);
 
    finalColor = vec4(mix(vec3(1.0), vec3(0.5), edgeFactor), 1.0);
}