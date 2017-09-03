#include "stdafx.h"
#include "meshShadowMap.h"
#include "anim.h"
#include "Importer.hpp"
#include "postprocess.h"
#include "TextureManager.h"
#include "OGLGE.h"

meshShadowMap::meshShadowMap()
{
	init();
}


meshShadowMap::~meshShadowMap()
{

}

void meshShadowMap::init()
{
	DefaultCamera->setFreeCamera(true);
	//DefaultCamera->setEyePosition(-50, 40, -50);
	//DefaultCamera->setTargetPosition(Vector3(0, 0, -50));

	m_tech = new shadowMapTechnique();
	m_tech->init();
	m_tech->enable();
	m_MVPMt4.identity();

	m_pipe.setCamera(DefaultCamera);
	m_pipe.WorldPos(0, 0, -5.0);
	m_pipe.Scale(10, 10, 10);

	m_pLightSpot = new Camera3D(Vector3(0.0, 0.0, 0.0), Vector3(0.0, 0.0, -1.0), Vector3(0.0, 1.0, 0.0));
	auto winrt = OGLGE::Instance()->getWindowsRect();
	m_pLightSpot->SetPerspectivePro(60, winrt.width, winrt.height, 2.0f, 100.0f);
	m_pLightSpot->setEyePosition(-50, 40, -50);	
	m_lightPipe.setCamera(m_pLightSpot);

	//init mesh node
	m_mesh = new MeshNode();
	m_mesh->setAttriPositionLoc(m_tech->positionLoc);
	m_mesh->setAttriTexCoordLoc(m_tech->texCoordLoc);
	m_mesh->setAttriNormalLoc(-1);

	initFBO();

	//init quad vbo ibo
	Vector3 Vertices[8];
	Vertices[0] = Vector3(-1.0f, 1.0f, 0.0f);
	Vertices[1] = Vector3(0.0f, 1.0f, 0.0f);

	Vertices[2] = Vector3(1.0f, 1.0f, 0.0f);
	Vertices[3] = Vector3(1.0f, 1.0f, 0.0f);

	Vertices[4] = Vector3(1.0f, -1.0f, 0.0f);
	Vertices[5] = Vector3(1.0f, 0.0f, 0.0f);

	Vertices[6] = Vector3(-1.0f, -1.0f, 0.0f);
	Vertices[7] = Vector3(0.0f, 0.0f, 0.0f);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	unsigned int Indices[] = {
		0, 2, 1,
		0, 3, 2,
	};
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

}

void meshShadowMap::initFBO()
{
	//create frame buffer and shadow texture
	glGenTextures(1, &m_shadowMap);
	glBindTexture(GL_TEXTURE_2D, m_shadowMap);
	auto winRt = OGLGE::Instance()->getWindowsRect();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, winRt.width, winRt.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (GL_FRAMEBUFFER_COMPLETE != status)
	{
		printf("init framebuffer error:0x%x\r\n", status);
		assert(0);
	}

	glBindTexture(GL_TEXTURE0, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void meshShadowMap::drawMesh()
{
	//reset depth buffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
	glClear(GL_DEPTH_BUFFER_BIT);
	

	glUniformMatrix4fv(m_tech->getUniformLocation("MVPMatrix"), 1, GL_TRUE, (const float*)m_lightMVPMt4.m);
	
	
	m_mesh->draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void meshShadowMap::drawShadowMap()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(m_tech->getUniformLocation("MVPMatrix"), 1, GL_TRUE, (const float*)m_MVPMt4.m);
	
	glEnableVertexAttribArray(m_tech->positionLoc);
	glEnableVertexAttribArray(m_tech->texCoordLoc);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(m_tech->positionLoc, 3, GL_FLOAT, GL_FALSE, 24, 0);
	glVertexAttribPointer(m_tech->texCoordLoc, 3, GL_FLOAT, GL_FALSE, 24, (GLvoid*)12);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_shadowMap);
	glUniform1i(m_tech->getUniformLocation("s_texture"), 0);// Set the sampler texture unit to 0

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(m_tech->positionLoc);
	glDisableVertexAttribArray(m_tech->texCoordLoc);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void meshShadowMap::draw()
{
	m_tech->enable();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	drawMesh();
	drawShadowMap();
}

void meshShadowMap::update(float ft)
{
	m_MVPMt4 = *m_pipe.GetTrans();
	m_lightMVPMt4 = *m_lightPipe.GetTrans();
}

void meshShadowMap::setPosition(float x, float y, float z)
{
	
	m_pLightSpot->setTargetPosition(Vector3(x, y, z));
	m_lightPipe.WorldPos(x, y, z);
}

void meshShadowMap::setScale(float s)
{
	
	m_lightPipe.Scale(s, s, s);
}

bool meshShadowMap::loadMesh(const char* filename)
{
	return m_mesh->loadMesh(filename);
}

void meshShadowMap::keyInput(unsigned char param, int x, int y)
{
	float factor = 0.5f;
	if (param == 'p')
	{
		auto epos = m_pLightSpot->getEyePosition();
		m_pLightSpot->setEyePosition(epos.x, epos.y, epos.z - 5);
	}
	if (param == 'o')
	{
		auto epos = m_pLightSpot->getEyePosition();
		m_pLightSpot->setEyePosition(epos.x, epos.y, epos.z + 5);
	}
}