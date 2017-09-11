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

	img_data jpgData;
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


unsigned int  TextureManager::loadTextureBmp(const char* filename, GLenum image_format, GLenum inernal_format, GLint level, GLint border)
{
	unsigned int texID = st_texIdSerial;

	if (st_texIdSerial == unsigned int(-1)) st_texIdSerial = 1;
	++st_texIdSerial;

	if (m_texList.find(texID) != m_texList.end())
	{
		printf("texture %d already exist\r\n", texID);
		return 0;
	}

	img_data jpgData;
	if (!TextureJpegLoader::Instance()->getBmpData(filename, jpgData))
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


unsigned int TextureManager::loadTextureCube(std::vector<std::string>& imgs, GLenum image_format, GLenum inernal_format, GLint level, GLint border)
{
	unsigned int texID = st_texIdSerial;

	if (st_texIdSerial == unsigned int(-1)) st_texIdSerial = 1;
	++st_texIdSerial;

	GLuint type[6];
	type[0] = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
	type[1] = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
	type[2] = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
	type[3] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
	type[4] = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
	type[5] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;

	GLuint textureID = 0;
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	img_data jpgData;
	for (int n = 0; n < 6; ++n)
	{
		if (!TextureJpegLoader::Instance()->getJpgDataEx(imgs[n].c_str(), jpgData))
		{
			printf("load jpeg texture failed:\r\n%s\r\n", imgs[n].c_str());
			return 0;
		}

		glTexImage2D(type[n], 0, GL_RGBA, jpgData._width, jpgData._height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, jpgData._data);
		
	}

	

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	m_texList[texID] = TextureData(textureID, jpgData._width, jpgData._height, image_format, imageExt::JPEG);
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

bool TextureManager::bindTexture(unsigned int texID, int target)
{
	if (m_texList.find(texID) != m_texList.end())
	{
		glBindTexture(target, m_texList[texID].id);
		return true;
	}
	return false;
}