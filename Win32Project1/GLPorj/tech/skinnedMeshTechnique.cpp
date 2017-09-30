#include "stdafx.h"
#include "skinnedMeshTechnique.h"
#include "OGLGE.h"

skinnedMeshTechnique::skinnedMeshTechnique()
{
}


skinnedMeshTechnique::~skinnedMeshTechnique()
{

}

void skinnedMeshTechnique::init()
{
	technique::init();
	const char* vshSrcFile = "res/shaderFile/skinnedMesh.vsh";
	const char* fshSrcFile = "res/shaderFile/skinnedMesh.fsh";
	addShader(vshSrcFile, fshSrcFile);

	positionLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_POSITION_NAME);
	texCoordLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_TEXCOORD_NAME);
	weightsLoc = glGetAttribLocation(m_shaderProg, "weights");
	boneIDsLoc = glGetAttribLocation(m_shaderProg, "boneIDs");
	
	char Name[128];
	for (unsigned int i = 0; i < MAX_BONES; i++) {
		sprintf_s(Name, sizeof(Name), "bonesTransformMt4[%d]", i);
		m_bonesMat4Loc[i] = getUniformLocationEx(Name);
	}

}

void skinnedMeshTechnique::setUniformBonesMat4(unsigned int n, const Matrix4f& mt4)
{
	if (n < MAX_BONES)
	{
		glUniformMatrix4fv(m_bonesMat4Loc[n], 1, GL_TRUE, (const float*)mt4.m);
	}
}