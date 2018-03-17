

#include "bumpNormalLightTechnique.h"


bumpNormalLightTechnique::bumpNormalLightTechnique()
{
	positionLoc = 0;
	texCoordLoc = 0;
	normalLoc = 0;
	tangentLoc = 0;
	bitangentLoc = 0;
}


bumpNormalLightTechnique::~bumpNormalLightTechnique()
{
}

void bumpNormalLightTechnique::init()
{
	technique::init();

	const char* vshSrcFile = "res/shaderFile/bumpNormalDiffuseLight.vsh";
	const char* fshSrcFile = "res/shaderFile/bumpNormalDiffuseLight.fsh";
	addShader(vshSrcFile, fshSrcFile);

	positionLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_POSITION_NAME);
	texCoordLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_TEXCOORD_NAME);
	normalLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_NORMAL_NAME);
	tangentLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_TANGENT_NAME);
	bitangentLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_BITANGENT_NAME);
	
}

