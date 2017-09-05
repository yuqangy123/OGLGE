#include "stdafx.h"
#include "shadowMapMeshTechnique.h"
#include "OGLGE.h"

shadowMapMeshTechnique::shadowMapMeshTechnique()
{
}


shadowMapMeshTechnique::~shadowMapMeshTechnique()
{
	if (nullptr != m_pLightSpot)
		delete m_pLightSpot;
	
	if(shadowMap>0)
		glDeleteTextures(1, &shadowMap);

	if (fbo > 0)
		glDeleteFramebuffers(1, &fbo);
}

void shadowMapMeshTechnique::init()
{
	technique::init();
	const char* vshSrcFile = "res/shaderFile/shadowmap.vsh";
	const char* fshSrcFile = "res/shaderFile/shadowmap.fsh";
	addShader(vshSrcFile, fshSrcFile);

	positionLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_POSITION_NAME);
	texCoordLoc = glGetAttribLocation(m_shaderProg, ATTRI_A_TEXCOORD_NAME);

	m_pLightSpot = new Camera3D(Vector3(0.0, 0.0, 0.0), Vector3(0.0, 0.0, -1.0), Vector3(0.0, 1.0, 0.0));
	auto winrt = OGLGE::Instance()->getWindowsRect();
	m_pLightSpot->SetPerspectivePro(60, winrt.width, winrt.height, 2.0f, 100.0f);
	m_pipe.setCamera(m_pLightSpot);

	initFBO();
}
void shadowMapMeshTechnique::setTargetPosition(float x, float y, float z)
{
	m_pLightSpot->setTargetPosition(Vector3(x, y, z));
}
void shadowMapMeshTechnique::setLightPosition(float x, float y, float z)
{
	m_pLightSpot->setEyePosition(x, y, z);
}


void shadowMapMeshTechnique::initFBO()
{
	//create frame buffer and shadow texture
	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	auto winRt = OGLGE::Instance()->getWindowsRect();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, winRt.width, winRt.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (GL_FRAMEBUFFER_COMPLETE != status)
	{
		printf("init framebuffer error:0x%x\r\n", status);
		assert(0);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void shadowMapMeshTechnique::renderFrameBuffer(std::vector<MeshNode*>& meshs)
{
	enable();

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	for (auto itr = meshs.begin(); itr != meshs.end(); ++itr)
	{
		MeshNode* m = (*itr);
		m_pipe.WorldPos(m->position.x, m->position.y, m->position.z);
		m_pipe.Scale(m->scale.x, m->scale.y, m->scale.z);
		glUniformMatrix4fv(getUniformLocation("MVPMatrix"), 1, GL_TRUE, (const float*)m_pipe.GetTrans()->m);
				
		m->setAttriPositionLoc(positionLoc);
		m->setAttriTexCoordLoc(texCoordLoc);
		m->setAttriNormalLoc(-1);
		m->draw();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
