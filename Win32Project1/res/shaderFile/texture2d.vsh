precision mediump float;
uniform mat4 	viewMatrix;
attribute vec4 	a_position;  
attribute vec2 	a_texCoord;
uniform float 	offsety;
varying vec2 v_texCoord;  
void main()  
{  
	gl_Position = viewMatrix * a_position;



	v_texCoord = a_texCoord;

}  