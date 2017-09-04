#include "stdafx.h"
#include "Texture2D.h"
#include "TextureManager.h"
#include "OGLGE.h"
#include "FileUnits.h"



Texture2D::Texture2D(const std::string& resPath)
{
	FileUnits::Instance()->getAbsoluteFilePath(resPath.c_str(), m_resourceFile);

	m_renderdata = nullptr;
	init();
}


Texture2D::~Texture2D()
{
	TextureMgr->unloadTexture(m_texturesID);
	m_texturesID = 0;

	if (nullptr != m_renderdata)
		delete []m_renderdata;
	m_renderdata = nullptr;
}

void Texture2D::setPosition(float x, float y)
{
	m_pipe.WorldPos(x, y, 0);
}

void Texture2D::setScale(float s)
{
	m_pipe.Scale(s, s, s);
}

void Texture2D::update(float ft)
{
}
void Texture2D::draw()
{
	Texture2DRender::Instance()->begin();
	Texture2DRender::Instance()->render(m_texturesID, m_renderdata, 4, *m_pipe.GetTrans());
	Texture2DRender::Instance()->end();
}

void Texture2D::init()
{
	//Texture2DRender::Instance();

	m_pipe.setCamera(DefaultCamera2D);

	m_texturesID = TextureMgr->loadTextureJpeg(m_resourceFile.c_str(), GL_RGBA, GL_RGBA, 0, 0);

	const TextureData& texData = TextureMgr->getTexture(m_texturesID);
	if (texData.id == 0)
	{
		printf("load jpg resource failed:\r\n%s\r\n", m_resourceFile.c_str());
		assert(0);
	}
	

	// Set the filtering mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	m_renderdata = new V3F_T2F[4];

	auto winRt = OGLGE::Instance()->getWindowsRect();
	m_position.x = -winRt.width / 2.0;
	m_position.y = -winRt.height / 2.0;
	m_renderdata[0].vertices = Vector3(m_position.x, m_position.y + texData.height, 0);
	m_renderdata[0].texCoords = Vector2(0, 0);
	m_renderdata[1].vertices = Vector3(m_position.x, m_position.y, 0);
	m_renderdata[1].texCoords = Vector2(0, 1);
	m_renderdata[2].vertices = Vector3(m_position.x + texData.width, m_position.y, 0);
	m_renderdata[2].texCoords = Vector2(1, 1);
	m_renderdata[3].vertices = Vector3(m_position.x + texData.width, m_position.y + texData.height, 0);
	m_renderdata[3].texCoords = Vector2(1, 0);

	
	/*
	m_renderdata[0].vertices = Vector3(-0.5f, 0.5f, 0.0f);
	m_renderdata[0].texCoords = Vector2(0, 0);
	m_renderdata[1].vertices = Vector3(-0.5f, -0.5f, 0.0f);
	m_renderdata[1].texCoords = Vector2(0, 1);
	m_renderdata[2].vertices = Vector3(0.5f, -0.5f, 0.0f);
	m_renderdata[2].texCoords = Vector2(1, 1);
	m_renderdata[3].vertices = Vector3(0.5f, 0.5f, 0.0f);
	m_renderdata[3].texCoords = Vector2(1, 0);
	*/
}