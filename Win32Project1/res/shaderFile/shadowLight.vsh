precision mediump float;

attribute vec3 	a_position;  
attribute vec3 	a_texCoord;
uniform mat4 	MVPMatrix;

attribute vec3	normal;
uniform mat4	WorldMatrix;
uniform mat4	lightWorldMatrix;

varying vec2 	v_texCoord;
varying vec3	w_normal;
varying vec4	lightSpacePos;


void main()  
{  
	gl_Position = MVPMatrix * vec4(a_position, 1.0);
	lightSpacePos = lightWorldMatrix * vec4(a_position, 1.0);
	
	v_texCoord = a_texCoord.xy; 

	w_normal = (WorldMatrix * vec4(normal, 0.0)).xyz;
}