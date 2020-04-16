#version 330

layout (triangles) in;
layout (line_strip, max_vertices=4) out;

// out vec3 posFrag;

uniform mat4 viewProjection;

void main() 
{
	for (int i = 0; i < gl_in.length(); ++i)
	{
		vec4 worldPos = gl_in[i].gl_Position;
		gl_Position = viewProjection * worldPos;
		// posFrag = (worldPos).xyz;
		EmitVertex();
	}

	vec4 worldPos = gl_in[0].gl_Position;
	gl_Position = viewProjection * worldPos;
	EmitVertex();
	
	EndPrimitive();
}
