#include "stdafx.h"
#include "geometryTechnique.h"


geometryTechnique::geometryTechnique()
{
	 positionLoc = 0;
	texCoordLoc = 0;
	normalLoc = 0;
}


geometryTechnique::~geometryTechnique()
{
}

void geometryTechnique::init()
{
	technique::init();

	const char* vshSrcFile = "res/shaderFile/GBuffer/geometry.vsh";
	const char* fshSrcFile = "res/shaderFile/GBuffer/geometry.fsh";
	addShader(vshSrcFile, fshSrcFile);
	
	positionLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_POSITION_NAME);
	texCoordLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_TEXCOORD_NAME);
	normalLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_NORMAL_NAME);
	
}

