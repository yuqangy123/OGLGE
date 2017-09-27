#include "stdafx.h"
#include "meshSkinnedAnimation.h"
#include "anim.h"
#include "Importer.hpp"
#include "postprocess.h"
#include "TextureManager.h"
#include "OGLGE.h"

meshSkinnedAnimation::meshSkinnedAnimation()
{
	init();
}


meshSkinnedAnimation::~meshSkinnedAnimation()
{
	if (m_mesh != nullptr)
		delete m_mesh;
}

void meshSkinnedAnimation::init()
{
	DefaultCamera->setFreeCamera(true);
	//DefaultCamera->setEyePosition(-50, 40, -50);
	//DefaultCamera->setTargetPosition(Vector3(0, 0, -50));

	m_tech = new skinnedMeshTechnique();
	m_tech->init();
	
	m_WVPMt4.identity();

	m_pipe.setCamera(DefaultCamera);
	//m_pipe.Rotate(-45, 0, 0);

	
}



void meshSkinnedAnimation::draw()
{
	m_tech->enable();

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(m_tech->getUniformLocation("WVPMatrix"), 1, GL_TRUE, (const float*)m_WVPMt4.m);

	const std::vector<Matrix4f>& bonesTransforms = m_mesh->getBonesTransformMt4();
	for (int i = 0; i < bonesTransforms.size(); ++i)
		m_tech->setUniformBonesMat4(i, bonesTransforms[i]);
	
	m_mesh->draw();
}

void meshSkinnedAnimation::update(float ft)
{
	m_WVPMt4 = *m_pipe.GetTrans();
}

void meshSkinnedAnimation::setPosition(float x, float y, float z)
{
	m_pipe.WorldPos(x, y, z);
}

void meshSkinnedAnimation::setScale(float s)
{
	m_pipe.Scale(s, s, s);
}

void meshSkinnedAnimation::Rotate(float x, float y, float z)
{
	m_pipe.Rotate(x, y, z);
}

bool meshSkinnedAnimation::loadMesh(const char* filename, const Vector3& pos, const Vector3& sal)
{
	m_mesh = new skinMeshNode();
	if (!m_mesh->loadMesh(filename))
		return false;

	m_mesh->position = pos;
	m_mesh->scale = sal;

	m_mesh->positionLoc = m_tech->positionLoc;
	m_mesh->texCoordLoc = m_tech->texCoordLoc;
	m_mesh->weightsLoc = m_tech->weightsLoc;
	m_mesh->boneIDsLoc = m_tech->boneIDsLoc;
}

void meshSkinnedAnimation::keyInput(unsigned char param, int x, int y)
{
	float factor = 0.5f;
	if (param == 'p')
	{
	}
	if (param == 'o')
	{
	}
}