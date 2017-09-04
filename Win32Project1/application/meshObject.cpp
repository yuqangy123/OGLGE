#include "stdafx.h"
#include "meshObject.h"
#include "anim.h"
#include "Importer.hpp"
#include "postprocess.h"
#include "TextureManager.h"
#include "OGLGE.h"

meshObject::meshObject()
{
	init();
}


meshObject::~meshObject()
{

}

void meshObject::init()
{
	
	m_tech = new lightTechnique();
	m_tech->init(lightType::diffuseLight);
	m_tech->enable();

	m_MVPMt4.identity();

	m_pipe.setCamera(DefaultCamera);


	m_ambientLightColor.set(0.5f, 0.5f, 0.5f);
	m_ambientLightIntensity = 0.5f;
	m_diffuseDirection.set(0.0f, -1.0f, 0.0f);
	m_diffuseDirection.normalize();
	m_diffuseIntensity = 3.0f;

	m_mesh = new MeshNode();
	m_mesh->setAttriPositionLoc(m_tech->positionLoc);
	m_mesh->setAttriTexCoordLoc(m_tech->texCoordLoc);
	m_mesh->setAttriNormalLoc(m_tech->normalLoc);
}

void meshObject::draw()
{
	m_tech->enable();


	glUniformMatrix4fv(m_tech->getUniformLocation("MVPMatrix"), 1, GL_TRUE, (const float*)m_MVPMt4.m);
	const Matrix4f* worldMt4 = m_pipe.GetWorldTrans();
	glUniformMatrix4fv(m_tech->getUniformLocation("WorldMatrix"), 1, GL_TRUE, (const float*)worldMt4->m);
	glUniform3f(m_tech->getUniformLocation("gDiffuseLight.color"), m_ambientLightColor.x, m_ambientLightColor.y, m_ambientLightColor.z);
	glUniform1f(m_tech->getUniformLocation("gDiffuseLight.ambientIntensity"), m_ambientLightIntensity);
	glUniform3f(m_tech->getUniformLocation("gDiffuseLight.direction"), m_diffuseDirection.x, m_diffuseDirection.y, m_diffuseDirection.z);
	glUniform1f(m_tech->getUniformLocation("gDiffuseLight.diffuseIntensity"), m_diffuseIntensity);

	m_mesh->draw();

}

void meshObject::update(float ft)
{
	m_MVPMt4 = *m_pipe.GetTrans();
}

void meshObject::setPosition(float x, float y, float z)
{
	m_pipe.WorldPos(x, y, z);
}

void meshObject::setScale(float s)
{
	m_pipe.Scale(s, s, s);
}

bool meshObject::loadMesh(const char* filename)
{
	return m_mesh->loadMesh(filename);
}

