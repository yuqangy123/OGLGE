precision mediump float;

attribute vec3 	a_position;  
attribute vec2 	a_texCoord;
attribute vec3	normal;
attribute vec3	tangent;

uniform mat4 	MVPMatrix;
uniform mat4	WorldMatrix;

varying vec2 	v_texCoord;
varying vec3	w_normal;
varying vec3	w_pixelPosition;
varying mat4	TBNMatrix;
void main()  
{  
	gl_Position = MVPMatrix * vec4(a_position, 1.0);
	v_texCoord = a_texCoord; 

	w_pixelPosition = (WorldMatrix * vec4(a_position, 1.0)).xyz;
	
	//w_normal = (WorldMatrix * vec4(normal, 0.0)).xyz;
	
	vec3 bittangent = dot(normal, tangent);
	
	TBNMatrix = (WorldMatrix * vec4(tangent, bittangent, normal, 0));
}