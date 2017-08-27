precision mediump float;

attribute vec3 	a_position;  
attribute vec2 	a_texCoord;
uniform mat4 	MVPMatrix;

attribute vec3	normal;
uniform mat4	WorldMatrix;

varying vec2 	v_texCoord;
varying vec3	w_normal;
varying vec3	w_pixelPosition;
void main()  
{  
	gl_Position = MVPMatrix * vec4(a_position, 1.0);
	v_texCoord = a_texCoord; 

	w_normal = (WorldMatrix * vec4(normal, 0.0)).xyz;
	w_pixelPosition = (WorldMatrix * vec4(a_position, 0.0)).xyz;
}