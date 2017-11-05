#include "stdafx.h"
#include "xFreeTypeFont.h"
#include "OGLGE.h"

using namespace uiSystem;

xFreeTypeFont::xFreeTypeFont()
{
	m_initGL = false;
	m_VBO = 0;
	m_IBO = 0;
}
xFreeTypeFont::~xFreeTypeFont()
{

}

xFreeTypeFont* xFreeTypeFont::create()
{
	xFreeTypeFont* ins = new xFreeTypeFont();
	return ins;
}

void xFreeTypeFont::setString(const wchar_t* str, int width, int height)
{
	m_str = str;
	m_rect.width = width;
	m_rect.height = height;

	if (!m_initGL)
	{
		/*
		m_StringTex = xFreeTypeLibIns.getStringTexture(str, width, height);
		
		

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		auto winRt = OGLGE::Instance()->getWindowsRect();
		int x = -winRt.width / 2.0;
		int y = -winRt.height / 2.0;
		GLfloat vVertices[] = { 
			x, y + m_StringTex.h, 0,
			0.0f,  0.0f,              // TexCoord 0 
			x, y, 0,
			0.0f,  1.0f,              // TexCoord 1
			x + m_StringTex.w, y, 0,
			1.0f,  1.0f,              // TexCoord 2
			x + m_StringTex.w, y + m_StringTex.h, 0,
			1.0f,  0.0f               // TexCoord 3
		};
		GLuint floatsz = sizeof(GLfloat);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_DYNAMIC_DRAW);		
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		glGenBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		GLushort _indices[6] = { 0, 1, 2, 0, 2, 3 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices[0]) * 6, _indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);		

		
		*/
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_IBO);
		m_tech.init();

		m_pipe.setCamera(DefaultCamera2D);
		m_initGL = true;

		FTFontManagerIns.initialize();
		char_font = FTFontManagerIns.createFont("res/jlx.ttf", 0, 32, false, false, true);
	}
}

void xFreeTypeFont::setPosition(float x, float y)
{
	m_pipe.WorldPos(x, y, 0);
}

void xFreeTypeFont::setScale(float s)
{
	m_pipe.Scale(s, s, s);
}




void xFreeTypeFont::update(float ft)
{

}

void xFreeTypeFont::draw_string(int x, int y, int font, wchar_t *string)
{
	if (!font)
		return;

	int tall = FTFontManagerIns.getFontTall(font);

	int dx = x;
	int dy = y;

	GLuint sglt = 0;

	m_tech.enable();

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glUniform1i(m_tech.getUniformLocation("s_texture"), 0);

	//vbo
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glEnableVertexAttribArray(m_tech.positionLoc);
	glEnableVertexAttribArray(m_tech.texCoordLoc);
	glVertexAttribPointer(m_tech.positionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)0);
	glVertexAttribPointer(m_tech.texCoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)12);

	//ibo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	GLushort _indices[6] = { 0, 1, 2, 0, 2, 3 };
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices[0]) * 6, _indices, GL_STATIC_DRAW);


	auto winRt = OGLGE::Instance()->getWindowsRect();
	int orgx = -winRt.width / 2.0;
	int orgy = -winRt.height / 2.0;


	while (*string)
	{
		if (*string == L'\n')
		{
			string++;
			dx = x;
			dy += tall + 2;    //row spacing
			continue;
		}

		int cw, ct, bx, by, av;
		GLuint glt;
		float crd[4];

		if (!FTFontManagerIns.getCharInfo(font, *string, &cw, &ct, &bx, &by, &av, &glt, crd))
		{
			string++;
			continue;
		}

		//大多数情况下多个字符都在同一个纹理中，避免频繁绑定纹理，可以提高效率
		if (glt != sglt)
		{
			glBindTexture(GL_TEXTURE_2D, glt);
			sglt = glt;
		}
		

		int px = dx + bx;
		int py = dy - by;

		//update vbo data
		GLfloat vVertices[] = {
			orgx, orgy + ct, 0,
			crd[0],  crd[1],              // TexCoord 0 
			orgx, orgy, 0,
			crd[0],  crd[3],              // TexCoord 1
			orgx + cw, orgy, 0,
			crd[2],  crd[3],              // TexCoord 2
			orgx + cw, orgy + ct, 0,
			crd[2],  crd[1]               // TexCoord 3
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_DYNAMIC_DRAW);

		//set mvp mt4
		m_pipe.WorldPos(px,py,0);
		glUniformMatrix4fv(m_tech.getUniformLocation("viewMatrix"), 1, GL_TRUE, (const float*)m_pipe.GetTrans()->m);
		
		
		//draw call
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

		dx += av;

		string++;
	}

	
}

void xFreeTypeFont::draw_page_texture(int x, int y, GLuint glt)
{
	if (!glt)
	{
		glDisable(GL_TEXTURE_2D);
		glColor4f(0.2, 0.2, 0.2, 1.0);
	}
	else
	{
		m_tech.enable();

		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, glt);
		glUniform1i(m_tech.getUniformLocation("s_texture"), 0);

		//vbo
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glEnableVertexAttribArray(m_tech.positionLoc);
		glEnableVertexAttribArray(m_tech.texCoordLoc);
		glVertexAttribPointer(m_tech.positionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)0);
		glVertexAttribPointer(m_tech.texCoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)12);

		//ibo
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		GLushort _indices[6] = { 0, 1, 2, 0, 2, 3 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices[0]) * 6, _indices, GL_STATIC_DRAW);


		auto winRt = OGLGE::Instance()->getWindowsRect();
		int orgx = -winRt.width / 2.0;
		int orgy = -winRt.height / 2.0;
		int cw = 256;
		int ct = 256;
		//update vbo data
		GLfloat vVertices[] = {
			orgx, orgy + ct, 0,
			0, 0,              // TexCoord 0 
			orgx, orgy, 0,
			0, 1,              // TexCoord 1
			orgx + cw, orgy, 0,
			1,  1,              // TexCoord 2
			orgx + cw, orgy + ct, 0,
			1,  0               // TexCoord 3
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_DYNAMIC_DRAW);

		//set mvp mt4
		m_pipe.WorldPos(x, y, 0);
		glUniformMatrix4fv(m_tech.getUniformLocation("viewMatrix"), 1, GL_TRUE, (const float*)m_pipe.GetTrans()->m);


		//draw call
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);


	}

}


void xFreeTypeFont::display()
{
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0, 1.0, 1.0, 0.5);

	draw_string(10, 30, char_font, (wchar_t*)m_str.c_str());

	//draw_page_texture(10, 350, 0);    //background
	draw_page_texture(10, 350, 1);    //page1 texture

	//draw_page_texture(276, 350, 0);    //background
	//draw_page_texture(276, 350, 2);    //page2 texture
	
	//close
	glDisableVertexAttribArray(m_tech.positionLoc);
	glDisableVertexAttribArray(m_tech.texCoordLoc);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisable(GL_BLEND);
}



void xFreeTypeFont::draw()
{
	if (!m_initGL)return;
	display();

	return;

	

	m_tech.enable();

	//混合
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//vbo
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glEnableVertexAttribArray(m_tech.positionLoc);
	glEnableVertexAttribArray(m_tech.texCoordLoc);
	glVertexAttribPointer(m_tech.positionLoc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)0);
	glVertexAttribPointer(m_tech.texCoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)12);

	//ibo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	
	//bind texture
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, m_StringTex.texObj);
	glUniform1i(m_tech.getUniformLocation("s_texture"), 0);

	//set mvp mt4
	glUniformMatrix4fv(m_tech.getUniformLocation("viewMatrix"), 1, GL_TRUE, (const float*)m_pipe.GetTrans()->m);

	//draw call
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	//close
	glDisableVertexAttribArray(m_tech.positionLoc);
	glDisableVertexAttribArray(m_tech.texCoordLoc);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisable(GL_BLEND);
}