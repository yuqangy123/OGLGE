

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

	const char* vshSrcFile = "res/shaderFile/skybox.vsh";
	const char* fshSrcFile = "res/shaderFile/skybox.fsh";
	addShader(vshSrcFile, fshSrcFile);	
	
	positionLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_POSITION_NAME);
}

