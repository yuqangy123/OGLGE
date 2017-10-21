#version 330

attribute vec3	a_position;
attribute vec3 	a_texCoord;
attribute vec3 	a_normal;

uniform  mat4  MVPMatrix;
uniform  mat4  WorldMatrix;

out vec3 position0;
out vec2 texCoord0;
out vec3 normal0;

void main()  
{
	gl_Position = MVPMatrix * vec4(a_position, 1.0);
	
	position0 = (WorldMatrix * vec4(a_position, 1.0)).xyz;
	texCoord0 = a_texCoord.xy;
	normal0 = (WorldMatrix * vec4(a_normal, 0.0)).xyz;
}  