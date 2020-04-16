#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

in vec3 normalGeo[];

out vec3 posFrag;
out vec3 normalFrag;

uniform mat4 viewProjection;

void main() 
{
	for (int i = 0; i < gl_in.length(); ++i)
	{
		vec4 worldPos = gl_in[i].gl_Position;
		gl_Position = viewProjection * worldPos;
		posFrag = (worldPos).xyz;
		normalFrag = normalGeo[i];
		EmitVertex();
	}
	
	EndPrimitive();
}