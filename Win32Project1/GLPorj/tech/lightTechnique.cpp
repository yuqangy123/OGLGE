#include "stdafx.h"
#include "lightTechnique.h"


lightTechnique::lightTechnique()
{
}


lightTechnique::~lightTechnique()
{
}

void lightTechnique::init(lightType tp)
{
	technique::init();

	switch (tp)
	{
		case lightType::ambientLight:
		{
			const char* vshSrcFile = "res/shaderFile/ambientLight.vsh";
			const char* fshSrcFile = "res/shaderFile/ambientLight.fsh";
			addShader(vshSrcFile, fshSrcFile);

			positionLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_POSITION_NAME);
			texCoordLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_TEXCOORD_NAME);

		}break;

		case lightType::diffuseLight:
		{
			const char* vshSrcFile = "res/shaderFile/diffuseLight.vsh";
			const char* fshSrcFile = "res/shaderFile/diffuseLight.fsh";
			addShader(vshSrcFile, fshSrcFile);

			positionLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_POSITION_NAME);
			texCoordLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_TEXCOORD_NAME);
			normalLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_NORMAL_NAME);

		}break;

		case lightType::specularLight:
		{
			const char* vshSrcFile = "res/shaderFile/specularLight.vsh";
			const char* fshSrcFile = "res/shaderFile/specularLight.fsh";
			addShader(vshSrcFile, fshSrcFile);

			positionLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_POSITION_NAME);
			texCoordLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_TEXCOORD_NAME);
			normalLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_NORMAL_NAME);

		}break;
	};

	
}

