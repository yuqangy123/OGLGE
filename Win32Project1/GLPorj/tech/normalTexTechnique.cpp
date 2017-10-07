#include "stdafx.h"
#include "normalTexTechnique.h"


normalTexTechnique::normalTexTechnique()
{
	 positionLoc = 0;
	texCoordLoc = 0;
}


normalTexTechnique::~normalTexTechnique()
{
}

void normalTexTechnique::init()
{
	technique::init();

	const char* vshSrcFile = "res/shaderFile/normalTex.vsh";
	const char* fshSrcFile = "res/shaderFile/normalTex.fsh";
	addShader(vshSrcFile, fshSrcFile);
	
	positionLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_POSITION_NAME);
	texCoordLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_TEXCOORD_NAME);
}

