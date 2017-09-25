precision mediump float;
uniform  mat4  gWorld;
uniform float offsetx;
attribute vec3 	a_position;
varying vec3 v_position;  

float clamp(float v)
{
	return v;
}

void main()  
{  
	
	gl_Position = gWorld * vec4(a_position, 1.0);
	
	v_position = vec3(clamp(gl_Position.x), clamp(gl_Position.y), clamp(gl_Position.z));


}  