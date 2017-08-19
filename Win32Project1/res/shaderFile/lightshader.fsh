precision mediump float;
varying vec4 OutColor;   
uniform sampler2D s_texture;

struct Light
{
	vec3 Color;
	float AmbientIntensity;
}

void main()
{
	gl_FragColor = OutColor;
}