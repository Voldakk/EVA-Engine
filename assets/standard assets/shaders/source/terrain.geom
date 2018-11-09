#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

out vec3 fragWireframeDist;

uniform mat4 view;
uniform mat4 projection;

void main(void)
{
    int i;
    for (i = 0; i < gl_in.length(); i++)
    {
        gl_Position = projection * view * gl_in[i].gl_Position;

		fragWireframeDist = vec3(0.0);
        fragWireframeDist[i] = 1.0;
        EmitVertex();
    }
}