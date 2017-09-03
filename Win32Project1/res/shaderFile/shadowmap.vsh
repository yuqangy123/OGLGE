precision mediump float;

attribute vec3 	a_position;  
attribute vec3 	a_texCoord;
uniform mat4 	MVPMatrix;


varying vec2 	v_texCoord;
void main()  
{  
	gl_Position = MVPMatrix * vec4(a_position, 1.0);
	v_texCoord = a_texCoord.xy; 

}