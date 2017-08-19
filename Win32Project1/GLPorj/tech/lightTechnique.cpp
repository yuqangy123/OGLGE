#include "stdafx.h"
#include "lightTechnique.h"


lightTechnique::lightTechnique()
{
}


lightTechnique::~lightTechnique()
{
}

void lightTechnique::init()
{
	technique::init();

	const char* vshSrcFile = "res/shaderFile/lightshader.vsh";
	const char* fshSrcFile = "res/shaderFile/lightshader.fsh";
	addShader(vshSrcFile, fshSrcFile);
	
	positionLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_POSITION_NAME);
	colorLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_COLOR_NAME);
	texCoordLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_TEXCOORD_NAME);
}

