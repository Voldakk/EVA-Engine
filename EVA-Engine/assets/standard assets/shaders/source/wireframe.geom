#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

in vec3 geomPos[];
in vec3 geomNormal[];
in vec2 geomTexCoord[];

out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoord;

out vec3 fragWireframeDist;

void main(void)
{
    int i;
    for (i = 0; i < gl_in.length(); i++)
    {
        fragPos = geomPos[i];
		fragNormal = geomNormal[i];
		fragTexCoord = geomTexCoord[i];
        gl_Position = gl_in[i].gl_Position;

		fragWireframeDist = vec3(0.0);
        fragWireframeDist[i] = 1.0;
        EmitVertex();
    }
}