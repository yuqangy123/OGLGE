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
	
	vec3 diffuseColor = vec3(0.0, 0.0, 0.0);
	vec3 normal = normalize(w_normal);
	float lightAng = dot(normal, -gDiffuseLight.direction);
	if(lightAng > 0)
	{
		diffuseColor = gDiffuseLight.color * gDiffuseLight.diffuseIntensity * lightAng;
	}
	
	vec3 ambientColor = gDiffuseLight.color * gDiffuseLight.ambientIntensity;
	ambientColor = ambientColor + diffuseColor;
	
	gl_FragColor = texColor * vec4(ambientColor, 1.0);
	
}