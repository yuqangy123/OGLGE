precision mediump float;

varying vec2 	v_texCoord;
varying	vec3	w_normal;

uniform sampler2D s_texture;

struct diffuseLight
{
	vec3 color;
    float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};
uniform diffuseLight gDiffuseLight;

void main()
{
	vec4 texColor = texture2D( s_texture, v_texCoord );
	
	vec3 diffuseColor = vec4(0, 0, 0, 0);
	float lightAng = dot(gDiffuseLight.direction, w_normal);
	if(lightAng > 0)
	{
		diffuseColor = gDiffuseLight.Color * gDiffuseLight.diffuseIntensity * lightAng;
	}
	
	vec3 ambientColor = gDiffuseLight.Color * gDiffuseLight.AmbientIntensity;
	ambientColor = ambientColor + diffuseColor;
	
	gl_FragColor = (texColor * vec4(ambientColor, 1.0f));
	
}