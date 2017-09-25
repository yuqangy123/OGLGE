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
	
	m_tech = new billboardTechnique();
	m_tech->init();
	
	m_pipe.setCamera(DefaultCamera);

	m_texture = TextureMgr->loadTextureJpeg("content/monster_hellknight.jpg", GL_RGBA, GL_RGBA, 0, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);

	Vector3 Vertices[] = {
		Vector3(.0f, 0.0f, -3.0f),
	};
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	//glSelectBuffer();
	//glPushMatrix();
}

void BillboardList::update(float ft)
{

}

void BillboardList::draw()
{
	m_tech->enable();

	auto cameraEye = DefaultCamera->getEyePosition();
	glUniformMatrix4fv(m_tech->getUniformLocation("gVP"), 1, GL_TRUE, (const float*)m_pipe.getVPTrans()->m);
	glUniform3f(m_tech->getUniformLocation("gCameraPos"), cameraEye.x, cameraEye.y, cameraEye.z);
	
	
	glActiveTexture(GL_TEXTURE0);
	TextureMgr->bindTexture(m_texture, GL_TEXTURE_2D);
	glUniform1i(m_tech->getUniformLocation("s_texture"), 0);

	

	glEnableVertexAttribArray(m_tech->positionLoc);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(m_tech->positionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_POINTS, 0, 1);

	glDisableVertexAttribArray(m_tech->positionLoc);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void BillboardList::setPosition(float x, float y, float z)
{
	m_pipe.WorldPos(x, y, z);
}

void BillboardList::setScale(float s)
{
	m_pipe.Scale(s, s, s);
}
