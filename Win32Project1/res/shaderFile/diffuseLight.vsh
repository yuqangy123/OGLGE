
attribute vec3 	a_position;  
attribute vec3 	a_texCoord;
attribute vec3	a_normal;

uniform mat4 	MVPMatrix;
uniform mat4	WorldMatrix;

varying vec2 	v_texCoord;
varying vec3	w_normal;
void main()  
{  
	gl_Position = MVPMatrix * vec4(a_position, 1.0);
	v_texCoord = a_texCoord.xy; 

	w_normal = (WorldMatrix * vec4(a_normal, 0.0)).xyz;
}