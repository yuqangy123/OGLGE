precision mediump float;
varying vec2 v_texCoord;
uniform sampler2D s_texture;

struct directionLight
{
	vec3 color;
    float ambientIntensity;
};
uniform directionLight gDirectionLight;

void main()
{
	vec4 texColor = texture2D( s_texture, v_texCoord );
	
	gl_FragColor = (texColor * vec4(gDirectionLight.Color, 1.0f)) * gDirectionLight.AmbientIntensity;	
}