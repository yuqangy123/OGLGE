#include "stdafx.h"
#include "BillboardList.h"
#include "anim.h"
#include "Importer.hpp"
#include "postprocess.h"
#include "TextureManager.h"
#include "OGLGE.h"

BillboardList::BillboardList()
{
	init();
}


BillboardList::~BillboardList()
{

}

void BillboardList::init()
{
	
	m_tech = new skyBoxTechnique();
	m_tech->init();
	
	m_pipe.setCamera(DefaultCamera);


	m_texture = TextureMgr->loadTextureJpeg("content/monster_hellknight.jpg", GL_RGBA, GL_RGBA, 0, 0);

	Vector3 Vertices[] = {
		Vector3(.0f, -3.0f, 0.0f),
	};
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	unsigned int Indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}

void BillboardList::update(float ft)
{

}

void BillboardList::draw()
{
	m_tech->enable();

	GLint OldCullFaceMode;
	glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
	GLint OldDepthFuncMode;
	glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

	glCullFace(GL_FRONT);
	//glDepthFunc(GL_LEQUAL);

	
	auto eyePos = DefaultCamera->getEyePosition();
	setPosition(eyePos.x, eyePos.y, eyePos.z);

	glUniformMatrix4fv(m_tech->getUniformLocation("MVPMatrix"), 1, GL_TRUE, (const float*)m_pipe.GetTrans()->m);
	
	glActiveTexture(GL_TEXTURE0);
	TextureMgr->bindTexture(m_texture, GL_TEXTURE_CUBE_MAP);
	glUniform1i(m_tech->getUniformLocation("s_texture"), 0);

	m_mesh->draw();

	glCullFace(OldCullFaceMode);
	glDepthFunc(OldDepthFuncMode);
}


void BillboardList::setPosition(float x, float y, float z)
{
	m_pipe.WorldPos(x, y, z);
}

void BillboardList::setScale(float s)
{
	m_pipe.Scale(s, s, s);
}
