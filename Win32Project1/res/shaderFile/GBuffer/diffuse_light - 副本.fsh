


uniform sampler2D s_positionMap;
uniform sampler2D s_diffuseMap;
uniform sampler2D s_normalMap;
uniform sampler2D s_texcoordMap;

uniform vec2 screenSize;


struct diffuseLight
{
	vec3 color;
    float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};
uniform diffuseLight gDiffuseLight;

vec2 calcTexCoord()
{	
	return vec2(gl_FragCoord.xy/screenSize);
}

void main()
{
	vec2 coord = calcTexCoord();
	
	vec4 texColor = texture2D( s_diffuseMap, coord );
	vec3 normal = texture2D( s_normalMap, coord ).xyz;
	
	vec3 diffuseColor = vec3(0.0, 0.0, 0.0);
	
	float diffuseFactor = dot(normal, -gDiffuseLight.direction);
	if(diffuseFactor > 0.0)
	{
		diffuseColor = gDiffuseLight.color * gDiffuseLight.diffuseIntensity * diffuseFactor;
	}
	
	vec3 ambientColor = texColor.xyz * gDiffuseLight.ambientIntensity;
	
	
	gl_FragColor = vec4(ambientColor + diffuseColor, 1.0);
	
}