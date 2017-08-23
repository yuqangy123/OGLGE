#pragma once
#include "Singleton.h"


typedef struct
{
	unsigned char*	_data;
	long			_width;
	long			_height;
}jpg_data;


class TextureJpegLoader :
	public CSingleton<TextureJpegLoader>
{
public:
	TextureJpegLoader();
	~TextureJpegLoader();

	bool initWithJpgData(const unsigned char * data, long dataLen, jpg_data& jpgData);
	bool getJpgData(const char* filename, jpg_data& jpgData);

	bool getJpgDataEx(const char* filename, jpg_data& jpgData);

protected:

};

