
attribute vec3 	a_position;
uniform mat4 	MVPMatrix;


void main()  
{  
	gl_Position = MVPMatrix * vec4(a_position, 1.0);
}