#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices=4) out;

out vec3 fragWireframeDist;

uniform mat4 viewProjection;

void main(void)
{
    for (int i = 0; i < gl_in.length(); ++i)
    {
        gl_Position = viewProjection * gl_in[i].gl_Position;

		fragWireframeDist = vec3(0.0);
        fragWireframeDist[i] = 1.0;
        EmitVertex();
    }

	gl_Position = viewProjection * gl_in[0].gl_Position;
    EmitVertex();
	
	EndPrimitive();
}