#include "stdafx.h"
#include "xFreeTypeLib.h"

#include <windows.h>//AnsiToUnicode
#include <locale.h>

xCharTexture g_TexID[65536];
xFreeTypeLib g_FreeTypeLib;


DECLARE_SINGLETON_MEMBER(xFreeTypeLib)
xFreeTypeLib::xFreeTypeLib()
{
	//����ʱ�趨��ǰANSI���룬VC��ʽ   
	setlocale(LC_ALL, ".936");

	//GCC�и�ʽ   
	setlocale(LC_ALL, "zh_CN.GBK");

	//VisualC++��ʹ��Сд%s������setlocaleָ������������ļ�   
	//GCC��ʹ�ô�д%S   
	g_FreeTypeLib.loadFont("res/arial.ttf", 12, 12);

	m_texCoords[0].x = 0; m_texCoords[0].y = 0;
	m_texCoords[1].x = 0; m_texCoords[1].y = 1;
	m_texCoords[2].x = 1; m_texCoords[2].y = 1;
	m_texCoords[3].x = 1; m_texCoords[3].y = 0;
	
	//m_tech = new texture2dTechnique();
	//m_tech->init();
}
xFreeTypeLib::~xFreeTypeLib()
{

}
void xFreeTypeLib::loadFont(const char* font_file, int _w, int _h)
{
	FT_Library library;
	if (FT_Init_FreeType(&library))
		exit(0);
	//����һ������,ȡĬ�ϵ�Face,һ��ΪRegualer
	if (FT_New_Face(library, font_file, 0, &m_FT_Face))
		exit(0);
	//ѡ���ַ���
	FT_Select_Charmap(m_FT_Face, FT_ENCODING_UNICODE);
	m_w = _w; m_h = _h;
	m_FT_Face->num_fixed_sizes;
	//��СҪ��64.���ǹ涨�������Ϳ����ˡ�
	FT_Set_Char_Size( m_FT_Face , 0 , m_w << 6, 96, 96);
	//�������ָ���ַ���Ⱥ͸߶ȵ��ض�����
	FT_Set_Pixel_Sizes(m_FT_Face, m_w, m_h);
}

void xFreeTypeLib::loadChar(wchar_t ch)
{
	if (g_TexID[ch].texData != nullptr)return;

	/* װ������ͼ�����βۣ�����Ĩ����ǰ������ͼ�� */
	if (0 != FT_Load_Char(m_FT_Face, ch, /*FT_LOAD_RENDER|*/FT_LOAD_FORCE_AUTOHINT |
		(true ? FT_LOAD_TARGET_NORMAL : FT_LOAD_MONOCHROME | FT_LOAD_TARGET_MONO)))
	{
		return;
	}

	if(FT_Load_Glyph( m_FT_Face, FT_Get_Char_Index( m_FT_Face, ch ), FT_LOAD_FORCE_AUTOHINT ))
		throw std::runtime_error("FT_Load_Glyph failed");

	xCharTexture& charTex = g_TexID[ch];

	//�õ���ģ
	FT_Glyph glyph;
	//������ͼ������β۸��Ƶ��µ�FT_Glyph����glyph�С������������һ�������벢������glyph�� 
	if (FT_Get_Glyph(m_FT_Face->glyph, &glyph))
		return ;

	//ת����λͼ
	FT_Render_Glyph(m_FT_Face->glyph, FT_RENDER_MODE_LCD);//FT_RENDER_MODE_NORMAL  ); 
	FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	//ȡ��λͼ����
	FT_Bitmap& bitmap = bitmap_glyph->bitmap;

	//��λͼ���ݿ����Լ��������������.�����ɿ��Ի�����Ҫ�Ķ��������ˡ�
	int width = bitmap.width;
	int height = bitmap.rows;

	m_FT_Face->size->metrics.y_ppem;		//�������뵽�豸�ռ�
	m_FT_Face->glyph->metrics.horiAdvance;  //ˮƽ�ı�����


	charTex.m_Width = width;
	charTex.m_Height = height;
	charTex.m_adv_x = m_FT_Face->glyph->advance.x / 64.0f;  //�������
	charTex.m_adv_y = m_FT_Face->glyph->advance.y / 64.0f; 		//m_FT_Face->size->metrics.y_ppem;
	charTex.m_delta_x = (float)bitmap_glyph->left;			//left:����ԭ��(0,0)������λͼ��������ص�ˮƽ����.�����������ص���ʽ��ʾ�� 
	charTex.m_delta_y = (float)bitmap_glyph->top - height;	//Top: ���������β۵�bitmap_top�ֶΡ�
	
	g_TexID[ch].texData = new char[width * height * 4];
	char* pBuf = g_TexID[ch].texData;
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			unsigned char _vl = (i >= bitmap.width || j >= bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width*j];
			pBuf[(4 * i + (j) * width * 4)] = 0xFF;
			pBuf[(4 * i + (j) * width * 4) + 1] = 0xFF;
			pBuf[(4 * i + (j) * width * 4) + 2] = 0xFF;
			pBuf[(4 * i + (j) * width * 4) + 3] = _vl;
			//pBuf[(4 * i + (height - j - 1) * width * 4) + 3] = _vl;
		}
	}
}

xCharTexture* getTextChar(wchar_t ch)
{
	g_FreeTypeLib.loadChar(ch);
	return &g_TexID[ch];
}

LPWSTR AnsiToUnicode(LPCSTR lpcstr)   //����lpcstr����Ҳ����char*
{
	LPWSTR Pwstr;
	int  i;
	i = MultiByteToWideChar(CP_ACP, 0, lpcstr, -1, NULL, 0);
	Pwstr = new WCHAR[i];
	MultiByteToWideChar(CP_ACP, 0, lpcstr, -1, Pwstr, i);

	return (Pwstr);
}


xStringTexture xFreeTypeLib::getStringTexture(const char* _strText, int maxW, int h)
{
	wchar_t *wstr = AnsiToUnicode(_strText);
	return getStringTexture(wstr, maxW, h);
}

xStringTexture xFreeTypeLib::getStringTexture(const wchar_t* _strText, int maxW, int h)
{
	xStringTexture res;

	int texW = 0, texH = 0;

	int x = 0, y = 0;
	int sx = x;
	int sy = y;
	int maxH = h;
	size_t nLen = wcslen(_strText);

	for (int i = 0; i <nLen; i++)
	{
		if (_strText[i] == '/n')
		{
			sx = x; sy += 12;
			continue;
		}

		xCharTexture* pCharTex = getTextChar(_strText[i]);															//glDisable(GL_TEXTURE_2D);
		int w = pCharTex->m_Width;
		int h = pCharTex->m_Height;

		int ch_x = sx + pCharTex->m_delta_x;
		int ch_y = sy + pCharTex->m_delta_y;

		if (maxH < h) break;
		
		if ((sx+w) > texW)texW = (sx+w);
		if ((sy+h) > texH)texH = (sy+h);

		sx += w;
		if (sx > x + maxW)
		{
			sx = x; sy += 12;
		}
	}
	unsigned char* pBuf = new unsigned char[texW*texH*4];
	memset(pBuf, 0x33, texW*texH * 4);
	
	sx = x;
	sy = y;
	maxH = h;
	for (int i = 0; i <nLen; i++)
	{
		if (_strText[i] == '/n')
		{
			sx = x; sy +=  12;
			continue;
		}
		xCharTexture* pCharTex = getTextChar(_strText[i]);		
		int w = pCharTex->m_Width;
		int h = pCharTex->m_Height;

		int ch_x = sx + pCharTex->m_delta_x;
		int ch_y = sy + pCharTex->m_delta_y;

		if (maxH < h) break;
		

		for (int col = 0; col < h; ++col)
		{
			int cs = col*w * 4;
			int bs = (col*texW + ch_x) * 4;
			memcpy(&pBuf[bs], pCharTex->texData+cs, w * 4);
		}

		sx += w;
		if (sx > x + maxW)
		{
			sx = x; sy +=  12;
		}
	}

	GLuint texObj = 0;
	glGenTextures(1, &texObj);
	glBindTexture(GL_TEXTURE_2D, texObj);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, pBuf);  //ָ��һ����ά������ͼƬ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);							   //glTexParameteri():�������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_REPLACE);								//������л��

	glBindTexture(GL_TEXTURE_2D, 0);
	delete pBuf;
	
	res.texObj = texObj;
	res.w = texW;
	res.h = texH;

	return res;
}