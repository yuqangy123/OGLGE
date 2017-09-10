precision mediump float;

attribute vec3 	a_position;  
attribute vec2 	a_texCoord;
attribute vec3	normal;
attribute vec3	tangent;

uniform mat4 	MVPMatrix;
uniform mat4	WorldMatrix;

varying vec2 	v_texCoord;
varying mat3	TBN;
varying vec3	WorldPosition0;


void main()  
{  
	gl_Position = MVPMatrix * vec4(a_position, 1.0);
	v_texCoord = a_texCoord; 

	
	vec3 Normal = normalize((WorldMatrix * vec4(normal, 0.0)).xyz);                                                       
    vec3 Tangent = normalize((WorldMatrix * vec4(tangent, 0.0)).xyz);                                                     
    Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);                           
    vec3 Bitangent = cross(Tangent, Normal);                              
    TBN = mat3(Tangent, Bitangent, Normal);
   	
	
	WorldPosition0 = (WorldMatrix * vec4(a_position, 1.0)).xyz;
}