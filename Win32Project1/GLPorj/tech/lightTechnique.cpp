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
			lightColorLoc = glGetAttribLocation(m_shaderProg, ATTRI_UNIFORM_DIRLIGHT_color);
			lightAmbientIntensityLoc = glGetAttribLocation(m_shaderProg, ATTRI_UNIFORM_DIRLIGHT_ambientIntensity);

		}break;

		case lightType::diffuseLight:
		{
				const char* vshSrcFile = "res/shaderFile/diffuseLight.vsh";
				const char* fshSrcFile = "res/shaderFile/diffuseLight.fsh";
				addShader(vshSrcFile, fshSrcFile);

				positionLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_POSITION_NAME);
				texCoordLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_TEXCOORD_NAME);
				lightColorLoc = glGetAttribLocation(m_shaderProg, ATTRI_UNIFORM_DIRLIGHT_color);
				lightAmbientIntensityLoc = glGetAttribLocation(m_shaderProg, ATTRI_UNIFORM_DIRLIGHT_ambientIntensity);
				lightDirectionLoc = glGetAttribLocation(m_shaderProg, ATTRI_UNIFORM_DIRLIGHT_direction);
				lightDiffuseIntensityLoc = glGetAttribLocation(m_shaderProg, ATTRI_UNIFORM_DIRLIGHT_diffuseIntensity);

		}break;
	};

	
}

