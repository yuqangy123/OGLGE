#include "stdafx.h"
#include "billboardTechnique.h"


billboardTechnique::billboardTechnique()
{
	positionLoc = -1;
}


billboardTechnique::~billboardTechnique()
{
}

void billboardTechnique::init()
{
	technique::init();

	const char* vshSrcFile = "res/shaderFile/skynetbox.vsh";
	const char* fshSrcFile = "res/shaderFile/skynetbox.fsh";
	addShader(vshSrcFile, fshSrcFile);	
	
	positionLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_POSITION_NAME);
}

