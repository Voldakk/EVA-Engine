#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

in vec2 uvGeo[];

out vec2 uvFrag;

uniform mat4 viewProjection;

void main(void)
{
    for (int i = 0; i < gl_in.length(); ++i)
    {
        gl_Position = viewProjection * gl_in[i].gl_Position;
        uvFrag = uvGeo[i];
        EmitVertex();
    }
	
	EndPrimitive();
}
