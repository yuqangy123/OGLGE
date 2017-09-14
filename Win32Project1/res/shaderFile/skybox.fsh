precision mediump float;

varying vec3 	v_texCoord;

uniform samplerCube s_texture;



void main()
{
	gl_FragColor = texture( s_texture, v_texCoord );
}