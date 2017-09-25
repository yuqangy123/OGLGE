precision mediump float;
varying vec2 texCoord0;   
uniform sampler2D s_texture;
void main()
{
	gl_FragColor = texture2D(s_texture, texCoord0);
	
}