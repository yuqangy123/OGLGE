#version 330

in vec3 position0;
in vec2 texCoord0;
in vec3 normal0;

layout (location = 0) out vec3 positionOut;
layout (location = 1) out vec3 diffuseOut;
layout (location = 2) out vec3 normalOut;
layout (location = 3) out vec3 texcoordOut;

uniform sampler2D s_texture;

void main()
{
	
	
	positionOut = position0;
	diffuseOut = (texture2D(s_texture, texCoord0)).rgb;
	normalOut = normalize(normal0);
	texcoordOut = vec3(texCoord0, 0.0);
}