#include "stdafx.h"
#include "texture2dTechnique.h"


texture2dTechnique::texture2dTechnique()
{
	positionLoc = 0;
	texCoordLoc = 0;
}


texture2dTechnique::~texture2dTechnique()
{
}

void texture2dTechnique::init()
{
	technique::init();

	const char* vshSrcFile = "res/shaderFile/texture2d.vsh";
	const char* fshSrcFile = "res/shaderFile/texture2d.fsh";
	addShader(vshSrcFile, fshSrcFile);
	
	positionLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_POSITION_NAME);
	texCoordLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_TEXCOORD_NAME);

	glBindAttribLocation(m_shaderProg, positionLoc, ATTRI_A_POSITION_NAME);
	glBindAttribLocation(m_shaderProg, texCoordLoc, ATTRI_A_TEXCOORD_NAME);
}

