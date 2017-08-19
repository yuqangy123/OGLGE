precision mediump float;
varying vec4 OutColor;   
uniform sampler2D s_texture;
void main()
{
	gl_FragColor = OutColor;
}