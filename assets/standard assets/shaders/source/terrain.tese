#version 430

layout (quads, equal_spacing, cw) in;

void main()
{
	float u = gl_TessCoord.x;
	float v = gl_TessCoord.y;

	vec4 position = 
	(
		(1-u) * (1-v) * gl_in[0].gl_Position + 
		(u)   * (1-v) * gl_in[1].gl_Position + 
		(u)   * (v)   * gl_in[3].gl_Position + 
		(1-u) * (v)   * gl_in[2].gl_Position
	);

	gl_Position = position;
}