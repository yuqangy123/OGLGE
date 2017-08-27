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
	vec3 lightDirection = -gDiffuseLight.direction;
	
	vec3 normal = normalize(w_normal);
	
	vec4 diffuseColor = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 specularColor = vec4(0.0, 0.0, 0.0, 1.0);
	
	float lightAng = dot(normal, lightDirection);
	if(lightAng > 0)
	{
		diffuseColor = vec4(gDiffuseLight.color * gDiffuseLight.diffuseIntensity * lightAng, 1.0f);
		
		vec3 eyeVector = normalize(gDiffuseLight.specularEye - w_pixelPosition);
		vec3 lightReflect = normalize(reflect(gDiffuseLight.direction, normal));
		float eyeLightAng = dot(eyeVector, lightReflect);
		if (eyeLightAng > 0)
		{
			eyeLightAng = pow(eyeLightAng, gDiffuseLight.specularPower);
			specularColor = vec4(gDiffuseLight.color * gDiffuseLight.specularIntensity * eyeLightAng, 1.0f);
		}
	}
	
	
	vec4 ambientColor = vec4(gDiffuseLight.color * gDiffuseLight.ambientIntensity, 1.0f);
	
	gl_FragColor = texColor * (ambientColor + diffuseColor + specularColor);
	
}