#pragma once
#include "GL/glew.h"
#include "Singleton.h"
#include "Vector2.h"
#include "Vector3.h"
#include "tech/texture2dTechnique.h"

#define  FT_FREETYPE_H <freetype/freetype.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

#define MAX_NO_TEXTURES 1
#define CUBE_TEXTURE 0

//GLuint texture_id[MAX_NO_TEXTURES];

typedef struct xCharTexture
{
	int     m_Width;
	int     m_Height;

	int     m_adv_x;
	int     m_adv_y;
	int     m_delta_x;
	int     m_delta_y;
	char*	texData;
public:
	xCharTexture()
	{
		m_Width = 0;
		m_Height = 0;
		texData = nullptr;
	}
};

typedef struct xStringTexture
{
	GLuint texObj;
	int w;
	int h;

	xStringTexture() :texObj(0), w(0), h(0) {};
};

#define xFreeTypeLibIns xFreeTypeLib::InstanceEx()
class xFreeTypeLib : public CSingleton<xFreeTypeLib>
{
public:
	xFreeTypeLib();
	~xFreeTypeLib();

	void loadFont(const char* font_file, int _w, int _h);
	void loadChar(wchar_t ch);

	xStringTexture getStringTexture(const char* _strText, int maxW, int h);
	xStringTexture getStringTexture(const wchar_t* _strText, int maxW, int h);

protected:
	FT_Library m_FT2Lib;
	FT_Face    m_FT_Face;
	int   m_w;
	int   m_h;
	Vector3     m_vertices[4];            // 12 bytes
	Vector2		m_texCoords[4];           // 8 bytes
	texture2dTechnique* m_tech;
};