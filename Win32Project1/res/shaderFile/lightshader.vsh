
uniform  mat4  gWorld;
uniform float offsetx;
attribute vec3 	a_position;
attribute vec3 	Color;
varying vec4 OutColor;  

float clamp(float v)
{
	if (v <= 0.0)
		return 1.0+v;
	return v;
}

void main()  
{  
	
	gl_Position = gWorld * vec4(a_position, 1.0);
	
	OutColor = vec4(Color, 1.0);


}  