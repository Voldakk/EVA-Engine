#version 430

layout(quads, fractional_odd_spacing, cw) in;

in vec2 uvTE[];
out vec2 uvGeo;

uniform sampler2D heightmap;
uniform float scaleY;

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
	
	// world position
	vec4 position =
	((1 - u) * (1 - v) * gl_in[12].gl_Position +
	    u    * (1 - v) * gl_in[ 0].gl_Position +
	    u    *    v    * gl_in[ 3].gl_Position +
	 (1 - u) *    v    * gl_in[15].gl_Position);

	uvGeo =
	((1 - u) * (1 - v) * uvTE[12] +
	    u    * (1 - v) * uvTE[ 0] +
	    u    *    v    * uvTE[ 3] +
	 (1 - u) *    v    * uvTE[15]);
	
	float height = texture(heightmap, uvGeo).r;

	position.y = height * scaleY;

	gl_Position = position;
}
