#pragma once
#include "Singleton.h"


typedef struct
{
	unsigned char*	_data;
	long			_width;
	long			_height;
}img_data;


class TextureJpegLoader :
	public CSingleton<TextureJpegLoader>
{
public:
	TextureJpegLoader();
	~TextureJpegLoader();

	bool initWithJpgData(const unsigned char * data, long dataLen, img_data& jpgData);
	bool getJpgData(const char* filename, img_data& jpgData);

	bool getJpgDataEx(const char* filename, img_data& jpgData);

	bool getBmpData(const char* filename, img_data& jpgData);

protected:

};

