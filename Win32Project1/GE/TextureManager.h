#pragma once
#include <string>
#include <map>
#include <vector>
#include "Singleton.h"
#include "GL/glew.h"
#include "GL/freeglut.h"

typedef enum {
	JPEG	=	1,
}imageExt;

typedef struct TextureData
{
	TextureData() :id(0), width(0), height(0) {};
	TextureData(GLuint _id, unsigned int _w, unsigned int _h, GLenum _img_format, imageExt _ext) :
		id(_id), width(_w), height(_h), image_format(_img_format), ext(_ext) {}

	GLuint			id;
	unsigned int	width;
	unsigned int	height;
	GLenum			image_format;
	imageExt		ext;
};

#define TextureMgr TextureManager::Instance()
class TextureManager : public CSingleton<TextureManager>
{
public:
	TextureManager();
	~TextureManager();
	
	unsigned int loadTextureJpeg(const char* filename, GLenum image_format, GLenum inernal_format, GLint level, GLint border);
	unsigned int loadTextureBmp(const char* filename, GLenum image_format, GLenum inernal_format, GLint level, GLint border);
	unsigned int loadTextureTga(const char* filename, GLenum image_format, GLenum inernal_format, GLint level, GLint border);
	unsigned int loadTextureCube(std::vector<std::string>& imgs, GLenum image_format, GLenum inernal_format, GLint level, GLint border);
	
	const TextureData& getTexture(unsigned int texID);
	void unloadTexture(unsigned int texID);
	bool bindTexture(unsigned int texID, int target = GL_TEXTURE_2D);

protected:
	unsigned int makeNewTexId();


protected:
	std::map<unsigned int, TextureData>	m_texList;
};

