

#include "shadowMeshTechnique.h"
#include "OGLGE.h"

shadowMeshTechnique::shadowMeshTechnique()
{
}


shadowMeshTechnique::~shadowMeshTechnique()
{

}

void shadowMeshTechnique::init()
{
	technique::init();
	const char* vshSrcFile = "res/shaderFile/shadowLight.vsh";
	const char* fshSrcFile = "res/shaderFile/shadowLight.fsh";
	addShader(vshSrcFile, fshSrcFile);

	positionLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_POSITION_NAME);
	texCoordLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_TEXCOORD_NAME);
	normalLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_NORMAL_NAME);

}
