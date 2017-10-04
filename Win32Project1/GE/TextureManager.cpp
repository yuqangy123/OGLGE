#include "stdafx.h"
#include "TextureManager.h"
#include "TextureJpegLoader.h"
#include "TextureTgaLoader.h"

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
	unsigned int texID = makeNewTexId();

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
	unsigned int texID = makeNewTexId();

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

unsigned int TextureManager::loadTextureTga(const char* filename, GLenum image_format, GLenum inernal_format, GLint level, GLint border)
{
	unsigned int texID = makeNewTexId();

	img_tga_data imgData;
	if (!TextureTgaLoader::Instance()->getTgaData(filename, imgData))
	{
		printf("load jpeg texture failed:\r\n%s\r\n", filename);
		return 0;
	}

	if (imgData.imageType == GL_RGB)
	{
		int buffLen = imgData.dataLen + imgData.dataLen / 3;
		GLubyte* buff = new GLubyte[buffLen];

		//convert to RGBA8888
		unsigned char* tdata = buff;
		GLubyte* pixels = imgData.imageData + imgData.dataLen - 3;
		for (int n = 0; n < imgData.dataLen - 2; n += 3)
		{
			*tdata++ = *(pixels + n + 0);
			*tdata++ = *(pixels + n + 1);
			*tdata++ = *(pixels + n + 2);
			*tdata++ = 0xFF;
			pixels -= 6;
		}
		delete imgData.imageData;

		for (int row = 0; row < imgData.height; ++row)
		{
			for (int col = 0; col < imgData.width/2; ++col)
			{
				int base = row*imgData.width;
				int a = (base + col) * 4;
				int b = (base + imgData.width - 1 - col) * 4;

				unsigned char tmpa0 = *(buff + a + 0);
				unsigned char tmpa1 = *(buff + a + 1);
				unsigned char tmpa2 = *(buff + a + 2);
				unsigned char tmpa3 = *(buff + a + 3);

				*(buff + a + 0) = *(buff + b + 0);
				*(buff + a + 1) = *(buff + b + 1);
				*(buff + a + 2) = *(buff + b + 2);
				*(buff + a + 3) = *(buff + b + 3);

				*(buff + b + 0) = tmpa0;
				*(buff + b + 1) = tmpa1;
				*(buff + b + 2) = tmpa2;
				*(buff + b + 3) = tmpa3;
			}
		}
		imgData.imageData = buff;
		imgData.dataLen = buffLen;
		imgData.imageType = GL_RGBA;
	}

	GLuint textureId = 0;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexImage2D(GL_TEXTURE_2D, level, inernal_format, imgData.width, imgData.height, border,
		image_format, GL_UNSIGNED_BYTE, imgData.imageData);

	m_texList[texID] = TextureData(textureId, imgData.width, imgData.height, image_format, imageExt::JPEG);
	return texID;
}

unsigned int TextureManager::loadTextureCube(std::vector<std::string>& imgs, GLenum image_format, GLenum inernal_format, GLint level, GLint border)
{
	unsigned int texID = makeNewTexId();

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

unsigned int TextureManager::makeNewTexId()
{
	unsigned int makeNewTexId();
	unsigned int texID = st_texIdSerial;

	if (st_texIdSerial == unsigned int(-1)) st_texIdSerial = 1;
	++st_texIdSerial;

	if (m_texList.find(texID) != m_texList.end())
	{
		printf("texture %d already exist\r\n", texID);
		return 0;
	}

	return texID;
}
