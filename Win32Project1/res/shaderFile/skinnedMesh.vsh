#version 330

const int MAX_BONES = 100;

layout(location=0) in vec3	a_position;
layout(location=1) in vec3	a_texCoord;
layout(location=2) in vec4	weights;
layout(location=3) in ivec4	boneIDs;


uniform  mat4  WVPMatrix;
uniform  mat4  bonesTransformMt4[MAX_BONES];


out vec2 texCoord0;  


void main()  
{  
	mat4 transform = bonesTransformMt4[boneIDs[0]] * weights[0];
	transform += bonesTransformMt4[boneIDs[1]] * weights[1];
	transform += bonesTransformMt4[boneIDs[2]] * weights[2];
	transform += bonesTransformMt4[boneIDs[3]] * weights[3];
	vec4 position = vec4(a_position, 1.0);
	position = transform * position;
	gl_Position = WVPMatrix * position;
	
	texCoord0 = a_texCoord.xy;


}  