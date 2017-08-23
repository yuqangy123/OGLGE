
uniform  mat4  gWorld;
attribute vec3	a_position;
attribute vec3 	texCoord;
varying vec2 texCoord0;  

float clamp(float v)
{
	if (v <= 0.0)
		return 1.0+v;
	return v;
}

void main()  
{  
	
	gl_Position = gWorld * vec4(a_position, 1.0);
	
	texCoord0 = texCoord.xy;


}  