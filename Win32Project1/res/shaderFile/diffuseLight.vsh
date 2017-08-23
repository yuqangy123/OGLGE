precision mediump float;

attribute vec3 	a_position;  
attribute vec2 	a_texCoord;
uniform mat4 	viewMatrix;

attribute vec3	normal;
uniform mat4	worldMatrix;

varying vec2 	v_texCoord;
varying vec3	w_normal;
void main()  
{  
	gl_Position = viewMatrix * vec4(a_position, 1.0);
	v_texCoord = a_texCoord; 

	w_normal = (worldMatrix * vec4(normal, 1.0)).xyz;
}  