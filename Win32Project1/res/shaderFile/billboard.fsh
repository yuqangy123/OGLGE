precision mediump float;

varying vec2 TexCoord;   
uniform sampler2D s_texture;

void main()
{
	gl_FragColor = texture2D(s_texture, TexCoord);
	
	if (gl_FragColor.r == 0 && gl_FragColor.g == 0 &&gl_FragColor.b == 0)
		discard;
	
}