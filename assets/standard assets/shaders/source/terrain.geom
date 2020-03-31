#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

struct Material
{
    sampler2D diffusemap;
    sampler2D normalmap;
    sampler2D heightmap;
    float heightScale;
    vec2 tiling;
};

in vec2 uvGeo[];

out vec2 uvFrag;
out vec3 posFrag;
out vec3 tangentFrag;

uniform mat4 viewProjection;
uniform sampler2D normalmap;
uniform vec3 cameraPosition;
uniform Material materials[2];
uniform int tbnRange;

vec3 tangent;
vec3 displacement[3];

void calcTangent()
{
    vec3 v0 = gl_in[0].gl_Position.xyz;
    vec3 v1 = gl_in[1].gl_Position.xyz;
    vec3 v2 = gl_in[2].gl_Position.xyz;

    vec3 e1 = v1 - v0;
    vec3 e2 = v2 - v0;

    vec2 uv0 = uvGeo[0];
    vec2 uv1 = uvGeo[1];
    vec2 uv2 = uvGeo[2];

    vec2 deltaUV1 = uv1 - uv0;
    vec2 deltaUV2 = uv2 - uv0;

    float r = 1.0 / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

    tangent = normalize((e1 * deltaUV2.y - e2 * deltaUV1.y) * r);
}

void main(void)
{
    for (int i = 0; i < 3; ++i)
    {
        displacement[i] = vec3(0, 0, 0);
    }

    float dist = (distance(gl_in[0].gl_Position.xyz, cameraPosition) + 
                  distance(gl_in[1].gl_Position.xyz, cameraPosition) + 
                  distance(gl_in[2].gl_Position.xyz, cameraPosition) / 3);

    if(dist < tbnRange)
    {
        calcTangent();

        for (int k = 0; k < gl_in.length(); ++k)
        {
            displacement[k] = vec3(0, 1, 0);

            float height = gl_in[k].gl_Position.y;

            vec3 normal = normalize(texture(normalmap, uvGeo[k]).rgb);

            float[2] materialAlpha = float[](0, 0);
            if(normal.y > 0.5)
            {
                materialAlpha[1] = 1;
            }
            else
            {
                materialAlpha[0] = 1;
            }

            float scale = 0;
            for(int i = 0; i < 2; ++i)
            {
                scale += texture(materials[i].heightmap, uvGeo[k] * materials[i].tiling).r * materials[i].heightScale * materialAlpha[i];
            }

            float attenuation = clamp(-distance(gl_in[k].gl_Position.xyz, cameraPosition) / (tbnRange - 50) + 1, 0.0, 1.0);
            scale *= attenuation;

            displacement[k] *= scale;
        }
    };

    for (int i = 0; i < gl_in.length(); ++i)
    {
        vec4 worldPos = gl_in[i].gl_Position + vec4(displacement[i], 0);
        gl_Position = viewProjection * worldPos;
        uvFrag = uvGeo[i];
        posFrag = worldPos.xyz;
        tangentFrag = tangent;
        EmitVertex();
    }
	
	EndPrimitive();
}
