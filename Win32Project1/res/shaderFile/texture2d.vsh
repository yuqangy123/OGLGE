precision mediump float;

attribute vec3 	a_position;  
attribute vec2 	a_texCoord;
uniform mat4 	viewMatrix;

varying vec2 v_texCoord;  
void main()  
{  
	gl_Position = viewMatrix * vec4(a_position, 1.0);
	v_texCoord = a_texCoord; 

}  