precision mediump float;
varying vec3 v_position;   
uniform sampler2D s_texture;
void main()
{
	
	gl_FragColor = vec4(v_position, 1.0);
}