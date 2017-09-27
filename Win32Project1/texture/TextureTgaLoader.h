#pragma once
#include "Singleton.h"
#include "GL/glew.h"

typedef struct _img_tga_data
{
	GLubyte*    imageData;				//memory need be safe release for yourself
	unsigned int  dataLen;
	GLuint        width;
	GLuint        height;
	GLuint        bpp;                //Image color depth in bits per pixel
	GLuint        texID;
	GLuint        imageType;
	GLboolean    bCompressed;        //Compressed or Uncompressed

	_img_tga_data() :imageData(NULL) {}
}img_tga_data;


class TextureTgaLoader :
	public CSingleton<TextureTgaLoader>
{
public:
	TextureTgaLoader();
	~TextureTgaLoader();

	bool getTgaData(const char* filename, img_tga_data& tgaData);
	bool loadCompressedTGA(img_tga_data* texture, FILE* file);
	bool loadUncompressedTGA(img_tga_data* texture, FILE* file);

protected:

};

