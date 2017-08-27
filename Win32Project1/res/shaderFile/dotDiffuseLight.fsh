precision mediump float;

varying vec2 	v_texCoord;
varying	vec3	w_normal;
varying	vec3	w_pixelPosition;

uniform sampler2D s_texture;

struct diffuseLight
{
	vec3 color;
    float ambientIntensity;
	
	float diffuseIntensity;
	
	vec3 lightPos;
	float weakContant;
	float weakLine;
};
uniform diffuseLight gDotDiffuseLight;

void main()
{
	vec4 texColor = texture2D( s_texture, v_texCoord );
	
	vec3 diffuseColor = vec3(0.0, 0.0, 0.0);
	vec3 normal = normalize(w_normal);
	vec3 lightpos = -normalize(gDotDiffuseLight.lightPos-w_pixelPosition);
	float diffuseFactor = dot(normal, lightpos);
	if(diffuseFactor > 0)
	{
		diffuseColor = gDotDiffuseLight.color * gDotDiffuseLight.diffuseIntensity * diffuseFactor;
		
	}
	
	vec3 ambientColor = gDotDiffuseLight.color * gDotDiffuseLight.ambientIntensity;
	ambientColor = ambientColor + diffuseColor;
	
	float lightPixelDistance = distance(w_pixelPosition, gDotDiffuseLight.lightPos);
	float distanceWeak = gDotDiffuseLight.weakContant + gDotDiffuseLight.weakLine*lightPixelDistance;
	ambientColor = ambientColor / distanceWeak;
	
	gl_FragColor = texColor * vec4(ambientColor, 1.0);
	
}