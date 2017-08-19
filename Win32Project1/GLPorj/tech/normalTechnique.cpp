#include "stdafx.h"
#include "normalTechnique.h"


normalTechnique::normalTechnique()
{
}


normalTechnique::~normalTechnique()
{
}

void normalTechnique::init()
{
	technique::init();

	const char* vshSrcFile = "res/shaderFile/normalshader.vsh";
	const char* fshSrcFile = "res/shaderFile/normalshader.fsh";
	addShader(vshSrcFile, fshSrcFile);
	
	positionLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_POSITION_NAME);
	colorLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_COLOR_NAME);
	texCoordLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_TEXCOORD_NAME);
}

