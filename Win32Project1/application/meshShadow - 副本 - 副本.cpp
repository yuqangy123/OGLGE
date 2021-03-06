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

	m_shadowMapTech=nullptr;
	m_tech = nullptr;
	
	DefaultCamera->setFreeCamera(true);
	DefaultCamera->setMouseCenterAlways(true);
	
	m_shadowMapTech = new shadowMapMeshTechnique();
	m_shadowMapTech->init();

	m_tech = new shadowMeshTechnique();
	m_tech->init();
	
	m_pipe.setCamera(DefaultCamera);

	/*
	//terrain box mesh
	m_terrainQuadMesh = new ModelMesh();
	m_terrainQuadMesh->loadMesh("content/box.obj");
	m_terrainQuadMesh->setAttriPositionLoc(m_tech->positionLoc);
	m_terrainQuadMesh->setAttriTexCoordLoc(m_tech->texCoordLoc);
	m_terrainQuadMesh->setAttriNormalLoc(m_tech->normalLoc);
	*/

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

	glClear( GL_DEPTH_BUFFER_BIT);

	m_tech->enable();
	
	glUniform3f(m_tech->getUniformLocation("gDiffuseLight.color"), m_ambientLightColor.x, m_ambientLightColor.y, m_ambientLightColor.z);
	glUniform1f(m_tech->getUniformLocation("gDiffuseLight.ambientIntensity"), m_ambientLightIntensity);
	glUniform3f(m_tech->getUniformLocation("gDiffuseLight.direction"), m_diffuseDirection.x, m_diffuseDirection.y, m_diffuseDirection.z);
	glUniform1f(m_tech->getUniformLocation("gDiffuseLight.diffuseIntensity"), m_diffuseIntensity);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_shadowMapTech->shadowMap);
	glUniform1i(m_tech->getUniformLocation("s_showmapTexture"), 1);
	glUniformMatrix4fv(m_tech->getUniformLocation("lightMVPMatrix"), 1, GL_TRUE, (const float*)m_shadowMapTech->getLightTrans()->m);
	
	for (auto& itr : m_meshs) {
		
		m_pipe.WorldPos(itr->position.x, itr->position.y, itr->position.z);
		m_pipe.Scale(itr->scale.x, itr->scale.y, itr->scale.z);

		glUniformMatrix4fv(m_tech->getUniformLocation("MVPMatrix"), 1, GL_TRUE, (const float*)m_pipe.GetTrans()->m);
		glUniformMatrix4fv(m_tech->getUniformLocation("WorldMatrix"), 1, GL_TRUE, (const float*)m_pipe.GetWorldTrans()->m);
		itr->draw();
	}
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
	ModelMesh* mesh = new ModelMesh();
	mesh->loadMesh(filename);
	mesh->position = pos;
	mesh->scale = sal;

	mesh->setAttriPositionLoc(m_tech->positionLoc);
	mesh->setAttriTexCoordLoc(m_tech->texCoordLoc);
	mesh->setAttriNormalLoc(m_tech->normalLoc);

	m_meshs.push_back(mesh);

	return true;
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