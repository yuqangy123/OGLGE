#include "stdafx.h"
#include "skyBoxTechnique.h"


skyBoxTechnique::skyBoxTechnique()
{
	positionLoc = -1;
}


skyBoxTechnique::~skyBoxTechnique()
{
}

void skyBoxTechnique::init()
{
	technique::init();

	const char* vshSrcFile = "res/shaderFile/skynetbox.vsh";
	const char* fshSrcFile = "res/shaderFile/skynetbox.fsh";
	addShader(vshSrcFile, fshSrcFile);	
	
	positionLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_POSITION_NAME);
}

