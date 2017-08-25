
uniform  mat4  MVPMatrix;
attribute vec3	a_position;
attribute vec3 	a_texCoord;
varying vec2 texCoord0;  


void main()  
{  
	
	gl_Position = MVPMatrix * vec4(a_position, 1.0);
	texCoord0 = a_texCoord.xy;


}  