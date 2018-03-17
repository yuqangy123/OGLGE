

#include "GBufferDiffuselightTechnique.h"


GBufferDiffuselightTechnique::GBufferDiffuselightTechnique()
{
	 positionLoc = 0;
}


GBufferDiffuselightTechnique::~GBufferDiffuselightTechnique()
{
}

void GBufferDiffuselightTechnique::init()
{
	technique::init();

	const char* vshSrcFile = "res/shaderFile/GBuffer/diffuse_light.vsh";
	const char* fshSrcFile = "res/shaderFile/GBuffer/diffuse_light.fsh";
	addShader(vshSrcFile, fshSrcFile);
	
	positionLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_POSITION_NAME);
	texCoordLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_TEXCOORD_NAME);
	normalLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_NORMAL_NAME);

}

