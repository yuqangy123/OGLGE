#pragma once
#include "Node.h"
#include <string>
#include "Rect.h"
#include "tech/texture2dTechnique.h"
#include "pipeLine.h"
#include "FTFontManager.h"


class xFreeTypeFont : public Node
{
public:
	xFreeTypeFont();
	~xFreeTypeFont();

	void setString(const wchar_t* str, int width=0, int height=0);
	void setPosition(float x, float y);
	void setScale(float s);

	void update(float ft);
	void draw();

protected:
	void draw_string(int x, int y, int font, wchar_t *string);
	void draw_page_texture(int x, int y, GLuint glt);
	void display();

protected:
	std::wstring m_str;
	Rect m_rect;
	texture2dTechnique m_tech;
	Pipeline		m_pipe;
	GLuint m_VBO;
	GLuint m_IBO;
	int char_font;
	bool m_initGL;
};