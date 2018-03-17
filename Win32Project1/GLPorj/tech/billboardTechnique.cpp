

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

	const char* vshSrcFile = "res/shaderFile/billboard.vsh";
	const char* fshSrcFile = "res/shaderFile/billboard.fsh";
	const char* gshSrcFile = "res/shaderFile/billboard.gsh";
	addShader(vshSrcFile, fshSrcFile, gshSrcFile);
	
	positionLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_POSITION_NAME);
}

