#include "stdafx.h"
#include "Texture2D.h"
#include "textureJpegManager.h"
#include "FileUnits.h"
#include "OGLGE.h"

Texture2D::Texture2D(const std::string& resPath)
{
	FileUnits::Instance()->getAbsoluteFilePath(resPath.c_str(), m_resourceFile);

	m_renderdata = nullptr;
	init();
}


Texture2D::~Texture2D()
{
	glDeleteTextures(1, &m_texturesID);
	m_texturesID = 0;

	if (nullptr != m_renderdata)
		delete []m_renderdata;
	m_renderdata = nullptr;
}

void Texture2D::setPosition(float x, float y)
{
	m_pipe.WorldPos(x, y, 0);
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
	m_pipe.setCamera(DefaultCamera2D);

	m_texturesID = 0;

	glGenTextures(1, &m_texturesID);
	glBindTexture(GL_TEXTURE_2D, m_texturesID);

	jpg_data jpgData;
	if (!textureJpegManager::Instance()->getJpgData(m_resourceFile.c_str(), jpgData))
	{
		printf("load jpg resource failed:\r\n%s\r\n", m_resourceFile.c_str());
		assert(0);
	}


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, jpgData._width, jpgData._height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, jpgData._data);

	// Set the filtering mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	m_renderdata = new V3F_T2F[4];

	auto winRt = OGLGE::Instance()->getWindowsRect();
	m_position.x = -winRt.width / 2.0;
	m_position.y = -winRt.height / 2.0;
	m_renderdata[0].vertices = Vector3(m_position.x, m_position.y + jpgData._height, 0);
	m_renderdata[0].texCoords = Vector2(0, 0);
	m_renderdata[1].vertices = Vector3(m_position.x, m_position.y, 0);
	m_renderdata[1].texCoords = Vector2(0, 1);
	m_renderdata[2].vertices = Vector3(m_position.x + jpgData._width, m_position.y, 0);
	m_renderdata[2].texCoords = Vector2(1, 1);
	m_renderdata[3].vertices = Vector3(m_position.x + jpgData._width, m_position.y + jpgData._height, 0);
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