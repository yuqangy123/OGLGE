#include "stdafx.h"
#include "TextureManager.h"
#include "TextureJpegLoader.h"

static unsigned int st_texIdSerial = 1;

DECLARE_SINGLETON_MEMBER(TextureManager);
TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}

unsigned int  TextureManager::loadTextureJpeg(const char* filename, GLenum image_format, GLenum inernal_format, GLint level, GLint border)
{
	unsigned int texID = st_texIdSerial;

	if (st_texIdSerial == unsigned int(-1)) st_texIdSerial = 1;
	++st_texIdSerial;

	if (m_texList.find(texID) != m_texList.end())
	{
		printf("texture %d already exist\r\n", texID);
		return 0;
	}

	jpg_data jpgData;
	if (!TextureJpegLoader::Instance()->getJpgDataEx(filename, jpgData))
	{
		printf("load jpeg texture failed:\r\n%s\r\n", filename);
		return 0;
	}
	GLuint textureId = 0;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexImage2D(GL_TEXTURE_2D, level, inernal_format, jpgData._width, jpgData._height, border,
		image_format, GL_UNSIGNED_BYTE, jpgData._data);

	m_texList[texID] = TextureData(textureId, jpgData._width, jpgData._height, image_format, imageExt::JPEG);
	return texID;
}

const TextureData& TextureManager::getTexture(unsigned int texID)
{
	if (m_texList.find(texID) != m_texList.end())
	{
		return m_texList[texID];
	}
	return TextureData();
}

void TextureManager::unloadTexture(unsigned int texID)
{
	if (m_texList.find(texID) != m_texList.end())
	{
		glDeleteTextures(1, &m_texList[texID].id);
		m_texList.erase(texID);
	}
}

bool TextureManager::bindTexture(unsigned int texID, int bid)
{
	if (m_texList.find(texID) != m_texList.end())
	{
		glBindTexture(bid, m_texList[texID].id);
		return true;
	}
	return false;
}