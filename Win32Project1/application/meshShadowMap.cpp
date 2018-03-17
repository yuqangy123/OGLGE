

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

	m_tech = new shadowMapMeshTechnique();
	m_tech->init();


	m_MVPMt4.identity();

	m_pipe.setCamera(DefaultCamera);
	//m_pipe.Rotate(-45, 0, 0);


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



void meshShadowMap::draw()
{
	m_tech->enable();
	m_tech->renderFrameBuffer(m_meshs);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(m_tech->getUniformLocation("MVPMatrix"), 1, GL_TRUE, (const float*)m_MVPMt4.m);

	glEnableVertexAttribArray(m_tech->positionLoc);
	glEnableVertexAttribArray(m_tech->texCoordLoc);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(m_tech->positionLoc, 3, GL_FLOAT, GL_FALSE, 24, 0);
	glVertexAttribPointer(m_tech->texCoordLoc, 3, GL_FLOAT, GL_FALSE, 24, (GLvoid*)12);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_tech->shadowMap);
	glUniform1i(m_tech->getUniformLocation("s_texture"), 0);// Set the sampler texture unit to 0

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(m_tech->positionLoc);
	glDisableVertexAttribArray(m_tech->texCoordLoc);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void meshShadowMap::update(float ft)
{
	m_MVPMt4 = *m_pipe.GetTrans();
}
void meshShadowMap::setLightTargetPosition(float x, float y, float z)
{
	m_tech->setTargetPosition(x, y, z);
}
void meshShadowMap::setLightPosition(float x, float y, float z)
{
	m_tech->setLightPosition(x, y, z);
}
void meshShadowMap::setPosition(float x, float y, float z)
{
	m_pipe.WorldPos(x, y, z);
}

void meshShadowMap::setScale(float s)
{
	m_pipe.Scale(s, s, s);
}

void meshShadowMap::Rotate(float x, float y, float z)
{
	m_pipe.Rotate(x, y, z);
}

bool meshShadowMap::loadMesh(const char* filename, const char* filepath, const Vector3& pos, const Vector3& sal)
{
	auto m = new ModelMesh();
	if (!m->loadMesh(filename, filepath))
		return false;

	m->position = pos;
	m->scale = sal;
	m_meshs.push_back(m);
}

void meshShadowMap::keyInput(unsigned char param, int x, int y)
{
	float factor = 0.5f;
	if (param == 'p')
	{
		auto epos = m_tech->getLightPosition();
		m_tech->setLightPosition(epos.x, epos.y, epos.z - 5);
	}
	if (param == 'o')
	{
		auto epos = m_tech->getLightPosition();
		m_tech->setLightPosition(epos.x, epos.y, epos.z + 5);
	}
}