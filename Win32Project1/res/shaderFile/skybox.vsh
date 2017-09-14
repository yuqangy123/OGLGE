precision mediump float;

attribute vec3 	a_position;
uniform mat4 	MVPMatrix;

varying vec3 v_texCoord;  
void main()  
{  
	gl_Position = MVPMatrix * vec4(a_position, 1.0);
	gl_Position = gl_Position.xyww;
	v_texCoord = a_position; 

}  