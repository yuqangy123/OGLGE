precision mediump float;

varying vec2 	v_texCoord;
varying	vec3	w_normal;
varying vec4	lightSpacePos;

uniform sampler2D s_texture;
uniform sampler2D s_showmapTexture;

struct diffuseLight
{
	vec3 color;
    float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};
uniform diffuseLight gDiffuseLight;

float calcShadowFactor()
{
	vec3 pos = lightSpacePos.xyz/lightSpacePos.w;
	pos.x = 0.5*pos.x + 0.5;
	pos.y = 0.5*pos.y + 0.5;
	pos.z = 0.5*pos.z + 0.5;
	vec2 lightCoordUV = pos.xy;
	
	float depth = texture2D( s_showmapTexture, lightCoordUV ).x;
	if(depth >= (pos.z + 0.00008))
		return 0.5;
	return 1.0;
}

void main()
{
	vec4 texColor = texture2D( s_texture, v_texCoord );
	
	vec3 diffuseColor = vec3(0.0, 0.0, 0.0);
	vec3 normal = normalize(w_normal);
	float diffuseFactor = dot(normal, -gDiffuseLight.direction);
	if(diffuseFactor > 0)
	{
		diffuseColor = gDiffuseLight.color * gDiffuseLight.diffuseIntensity * diffuseFactor;
	}
	
	vec3 ambientColor = gDiffuseLight.color * gDiffuseLight.ambientIntensity;
	ambientColor = ambientColor + diffuseColor*calcShadowFactor();
	
	gl_FragColor = texColor * vec4(ambientColor, 1.0);
	
}