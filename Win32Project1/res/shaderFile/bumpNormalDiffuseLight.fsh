precision mediump float;

varying vec2 	v_texCoord;
varying mat3	TBN;
varying vec3	WorldPosition0;


uniform sampler2D s_texture;
uniform sampler2D s_normalTexture;

uniform float switchNormal;

struct diffuseLight
{
	vec3 color;
    float ambientIntensity;
	float diffuseIntensity;
	
	vec3 lightPos;
	float weakContant;
	float weakLine;
};
uniform diffuseLight gDiffuseLight;

vec3 CalcBumpedNormal()
{                                             
    vec3 BumpMapNormal = texture(s_normalTexture, v_texCoord).xyz;                                
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);                              
    
	vec3 NewNormal;
	NewNormal = TBN * BumpMapNormal;                                                        
	return normalize(NewNormal);
}

void main()
{
	vec4 texColor = texture2D( s_texture, v_texCoord );
	
	vec3 diffuseColor = vec3(0.0, 0.0, 0.0);
	vec3 normal = CalcBumpedNormal();
	vec3 lightDireciton = gDiffuseLight.lightPos - WorldPosition0;
	float diffuseFactor = dot(normal, lightDireciton);
	if(diffuseFactor > 0)
	{
		diffuseColor = gDiffuseLight.color * gDiffuseLight.diffuseIntensity * diffuseFactor;
	}
	
	vec3 ambientColor = gDiffuseLight.color * gDiffuseLight.ambientIntensity;
	ambientColor = ambientColor + diffuseColor;
	
	float lightPixelDistance = distance(WorldPosition0, gDiffuseLight.lightPos);
	float distanceWeak = gDiffuseLight.weakContant + gDiffuseLight.weakLine*lightPixelDistance;
	ambientColor = ambientColor / distanceWeak;
	
	gl_FragColor = texColor * vec4(ambientColor, 1.0);
	
}