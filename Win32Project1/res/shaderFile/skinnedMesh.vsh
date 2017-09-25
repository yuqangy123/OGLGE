
const int MAX_BONES = 100;

layout(location=0) in vec3	a_position;
layout(location=1) in vec3	a_texCoord;
layout(location=2) in vec4	weights;
layout(location=3) in vec4	boneIDs;
layout(location=3) in mat4[MAX_BONES] bonesTransformMt4;


uniform  mat4  WVPMatrix;



out vec2 texCoord0;  


void main()  
{  
	mat4 transform = bonesTransformMt4[boneIDs.x] * weights.x;
	transform = transform + bonesTransformMt4[boneIDs.y] * weights.y;
	transform = transform + bonesTransformMt4[boneIDs.z] * weights.z;
	transform = transform + bonesTransformMt4[boneIDs.w] * weights.w;
	vec4 position = vec4(a_position, 1.0);
	position = transform * position;
	gl_Position = gWorld * position;
	
	texCoord0 = a_texCoord.xy;


}  