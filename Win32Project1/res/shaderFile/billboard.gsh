#version 330

layout(points) in;                                                                  
layout(triangle_strip) out;                                                         
layout(max_vertices = 4) out;                                                       
                                                                                    
uniform mat4 gVP;                                                                   
uniform vec3 gCameraPos;                                                            
                                                                                    
out vec2 TexCoord;     

void main()  
{  
	vec3 Pos = gl_in[0].gl_Position.xyz;
		
	vec3 frontVtr = normalize(gCameraPos - Pos);
	vec3 upVtr = vec3(0.0, 1.0, 0.0);
	vec3 rightVtr = normalize(cross(frontVtr, upVtr));
	
	Pos = Pos - rightVtr*0.5;
	gl_Position = gVP * vec4(Pos, 1.0);
	TexCoord = vec2(0.0, 0.0);
	EmitVertex();
	
	Pos = Pos + upVtr*1.0;
	gl_Position = gVP * vec4(Pos, 1.0);
	TexCoord = vec2(0.0, 1.0);
	EmitVertex();
	
	Pos = Pos + rightVtr*1.0;
	Pos = Pos - upVtr*1.0;
	gl_Position = gVP * vec4(Pos, 1.0);
	TexCoord = vec2(1.0, 0.0);
	EmitVertex();
	
	
	Pos = Pos + upVtr*1.0;
	gl_Position = gVP * vec4(Pos, 1.0);
	TexCoord = vec2(1.0, 1.0);
	EmitVertex();
	
	
	EndPrimitive();
}  