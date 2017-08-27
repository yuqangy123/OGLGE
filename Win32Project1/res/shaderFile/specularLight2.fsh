precision mediump float;

varying vec2 	v_texCoord;
varying	vec3	w_normal;
varying	vec3	w_pixelPosition;

uniform sampler2D s_texture;

struct diffuseLight
{
	vec3 color;
    float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
	vec3 specularEye;
	float specularPower;
	float specularIntensity;
};
uniform diffuseLight gDiffuseLight;

void main()
{
	vec4 texColor = texture2D( s_texture, v_texCoord );
	
	vec3 diffuseColor = vec3(0.0, 0.0, 0.0);
	vec3 specularColor = vec3(0.0, 0.0, 0.0);
	vec3 normal = normalize(w_normal);
	float lightAng = dot(normal, -gDiffuseLight.direction);
	if(lightAng > 0)
	{
		diffuseColor = gDiffuseLight.color * gDiffuseLight.diffuseIntensity * lightAng;
		
		vec3 eyeVector = normalize(gDiffuseLight.specularEye - w_pixelPosition);
		vec3 lightReflect = normalize(reflect(gDiffuseLight.direction, normal));
		float eyeLightAng = dot(eyeVector, lightReflect);
		if (eyeLightAng > 0)
		{
			eyeLightAng = pow(eyeLightAng, gDiffuseLight.specularPower);
			specularColor = gDiffuseLight.color * gDiffuseLight.specularIntensity * eyeLightAng;
		}
	}
	
	
	vec3 ambientColor = gDiffuseLight.color * gDiffuseLight.ambientIntensity;
	ambientColor = ambientColor + diffuseColor + specularColor;
	
	gl_FragColor = texColor * vec4(ambientColor, 1.0);
	
}