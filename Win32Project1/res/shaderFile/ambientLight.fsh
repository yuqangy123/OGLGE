precision mediump float;
varying vec2 texCoord0;
uniform sampler2D s_texture;

struct directionLight
{
	vec3 color;
    float ambientIntensity;
};
uniform directionLight gDirectionLight;

void main()
{
	vec4 texColor = texture2D( s_texture, texCoord0 );
	
	gl_FragColor = texColor * vec4(gDirectionLight.color, 1.0f) * gDirectionLight.ambientIntensity;	
}