#pragma once
#include "Singleton.h"


typedef struct
{
	unsigned char*	_data;
	long			_width;
	long			_height;
}jpg_data;


class textureJpegManager :
	public CSingleton<textureJpegManager>
{
public:
	textureJpegManager();
	~textureJpegManager();

	bool initWithJpgData(const unsigned char * data, long dataLen, jpg_data& jpgData);
	bool getJpgData(const char* filename, jpg_data& jpgData);

	bool getJpgDataEx1(const char* filename, jpg_data& jpgData);

protected:

};

