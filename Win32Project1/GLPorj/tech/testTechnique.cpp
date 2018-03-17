

#include "testTechnique.h"


testTechnique::testTechnique()
{
}


testTechnique::~testTechnique()
{
}

void testTechnique::init()
{
	technique::init();

	const char* vshSrcFile = "res/shaderFile/test.vsh";
	const char* fshSrcFile = "res/shaderFile/test.fsh";
	addShader(vshSrcFile, fshSrcFile);
	
	positionLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_POSITION_NAME);
	
}

