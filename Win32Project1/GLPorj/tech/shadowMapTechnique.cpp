#include "stdafx.h"
#include "shadowMapTechnique.h"


shadowMapTechnique::shadowMapTechnique()
{
}


shadowMapTechnique::~shadowMapTechnique()
{
}

void shadowMapTechnique::init()
{
	technique::init();
	const char* vshSrcFile = "res/shaderFile/shadowmap.vsh";
	const char* fshSrcFile = "res/shaderFile/shadowmap.fsh";
	addShader(vshSrcFile, fshSrcFile);

	positionLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_POSITION_NAME);
	texCoordLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_TEXCOORD_NAME);
}

