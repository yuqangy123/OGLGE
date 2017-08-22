precision mediump float;

varying vec2 	v_texCoord;
varying	vec3	w_normal;

uniform sampler2D s_texture;

struct diffuseLight
{
	vec3 Color;
    float AmbientIntensity;
	vec3 direction;
	float diffuseIntensity;
};
uniform diffuseLight gDiffuseLight;

void main()
{
	vec4 texColor = texture2D( s_texture, v_texCoord );
	
	float lightAng = dot(gDiffuseLight.direction, w_normal);
	vec4 diffuseColor = vec4(1,1,1,0);
	
	if(lightAng < 90.0 && lightAng > -90.0)
	{
		diffuseColor
	}
	
	vec4 directionColor = (texColor * vec4(gDiffuseLight.Color, 1.0f)) * gDiffuseLight.AmbientIntensity;
	gl_FragColor
}