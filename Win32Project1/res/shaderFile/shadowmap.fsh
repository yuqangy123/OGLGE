precision mediump float;

varying vec2 	v_texCoord;

uniform sampler2D s_texture;



void main()
{
	vec4 mapDepth = texture2D( s_texture, v_texCoord ).x;
	float depth = 1.0 - (mapDepth)*25.0;
	gl_FragColor = vec4(depth);
	
}