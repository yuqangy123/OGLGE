#include "stdafx.h"
#include "meshShadow.h"
#include "anim.h"
#include "Importer.hpp"
#include "postprocess.h"
#include "TextureManager.h"
#include "OGLGE.h"

meshShadow::meshShadow()
{
	init();
}


meshShadow::~meshShadow()
{

}

void meshShadow::init()
{
	DefaultCamera->setFreeCamera(true);
	
	m_shadowMapTech = new shadowMapMeshTechnique();
	m_shadowMapTech->init();

	m_tech = new shadowMeshTechnique();
	m_tech->init();

	

	m_pipe.setCamera(DefaultCamera);

	//terrain box mesh
	m_terrainQuadMesh = new MeshNode();
	m_terrainQuadMesh->loadMesh("content/box.obj");
	m_terrainQuadMesh->setAttriPositionLoc(m_tech->positionLoc);
	m_terrainQuadMesh->setAttriTexCoordLoc(m_tech->texCoordLoc);
	m_terrainQuadMesh->setAttriNormalLoc(m_tech->normalLoc);

	m_ambientLightColor.set(0.5f, 0.5f, 0.5f);
	m_ambientLightIntensity = 0.5f;
	m_diffuseDirection.set(0.0f, -1.0f, 0.0f);
	m_diffuseDirection.normalize();
	m_diffuseIntensity = 3.0f;
}



void meshShadow::draw()
{
	m_shadowMapTech->enable();
	m_shadowMapTech->renderFrameBuffer(m_meshs);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_tech->enable();
	glUniformMatrix4fv(m_tech->getUniformLocation("MVPMatrix"), 1, GL_TRUE, (const float*)m_pipe.GetTrans()->m);
	glUniformMatrix4fv(m_tech->getUniformLocation("WorldMatrix"), 1, GL_TRUE, (const float*)m_pipe.GetWorldTrans()->m);
	glUniform3f(m_tech->getUniformLocation("gDiffuseLight.color"), m_ambientLightColor.x, m_ambientLightColor.y, m_ambientLightColor.z);
	glUniform1f(m_tech->getUniformLocation("gDiffuseLight.ambientIntensity"), m_ambientLightIntensity);
	glUniform3f(m_tech->getUniformLocation("gDiffuseLight.direction"), m_diffuseDirection.x, m_diffuseDirection.y, m_diffuseDirection.z);
	glUniform1f(m_tech->getUniformLocation("gDiffuseLight.diffuseIntensity"), m_diffuseIntensity);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_shadowMapTech->shadowMap);
	glUniform1i(m_tech->getUniformLocation("s_showmapTexture"), 1);
	glUniformMatrix4fv(m_tech->getUniformLocation("lightWorldMatrix"), 1, GL_TRUE, (const float*)m_shadowMapTech->getLightWorldTrans()->m);

	m_terrainQuadMesh->draw();
	if(nullptr != m_mesh)m_mesh->draw();
}

void meshShadow::update(float ft)
{
}

void meshShadow::updateLightDirection()
{
	const Vector3& targetPos = m_shadowMapTech->getTargetPosition();
	const Vector3& eyePos = m_shadowMapTech->getLightPosition();
	m_diffuseDirection.set(targetPos.x - eyePos.x, targetPos.y - eyePos.y, targetPos.z - eyePos.z);
	m_diffuseDirection.normalize();
}

void meshShadow::setTargetPosition(float x, float y, float z)
{
	m_shadowMapTech->setTargetPosition(x, y, z);
	updateLightDirection();
}

void meshShadow::setLightPosition(float x, float y, float z)
{
	m_shadowMapTech->setLightPosition(x, y, z);
	updateLightDirection();
}

bool meshShadow::loadMesh(const char* filename, const Vector3& pos, const Vector3& sal)
{
	if (m_mesh == nullptr)
		m_mesh = new MeshNode();

	if (m_mesh->isLoaded() || !m_mesh->loadMesh(filename))
		return false;

	m_mesh->position = pos;
	m_mesh->scale = sal;
	m_meshs.push_back(m_mesh);

	m_mesh->setAttriPositionLoc(m_tech->positionLoc);
	m_mesh->setAttriTexCoordLoc(m_tech->texCoordLoc);
	m_mesh->setAttriNormalLoc(m_tech->normalLoc);

	m_pipe.WorldPos(pos.x, pos.y, pos.z);
	m_pipe.Scale(sal.x, sal.y, sal.z);
}

void meshShadow::keyInput(unsigned char param, int x, int y)
{
	float factor = 0.5f;
	if (param == 'p')
	{
		//auto epos = m_tech->getLightPosition();
		//m_tech->setLightPosition(epos.x, epos.y, epos.z - 5);
	}
	if (param == 'o')
	{
		//auto epos = m_tech->getLightPosition();
		//m_tech->setLightPosition(epos.x, epos.y, epos.z + 5);
	}
}