#pragma once
#include "Singleton.h"

enum class PixelFormat
{
	//! auto detect the type
	AUTO,
	//! 32-bit texture: BGRA8888
	BGRA8888,
	//! 32-bit texture: RGBA8888
	RGBA8888,
	//! 24-bit texture: RGBA888
	RGB888,
	//! 16-bit texture without Alpha channel
	RGB565,
	//! 8-bit textures used as masks
	A8,
	//! 8-bit intensity texture
	I8,
	//! 16-bit textures used as masks
	AI88,
	//! 16-bit textures: RGBA4444
	RGBA4444,
	//! 16-bit textures: RGB5A1
	RGB5A1,
	//! 4-bit PVRTC-compressed texture: PVRTC4
	PVRTC4,
	//! 4-bit PVRTC-compressed texture: PVRTC4 (has alpha channel)
	PVRTC4A,
	//! 2-bit PVRTC-compressed texture: PVRTC2
	PVRTC2,
	//! 2-bit PVRTC-compressed texture: PVRTC2 (has alpha channel)
	PVRTC2A,
	//! ETC-compressed texture: ETC
	ETC,
	//! S3TC-compressed texture: S3TC_Dxt1
	S3TC_DXT1,
	//! S3TC-compressed texture: S3TC_Dxt3
	S3TC_DXT3,
	//! S3TC-compressed texture: S3TC_Dxt5
	S3TC_DXT5,
	//! ATITC-compressed texture: ATC_RGB
	ATC_RGB,
	//! ATITC-compressed texture: ATC_EXPLICIT_ALPHA
	ATC_EXPLICIT_ALPHA,
	//! ATITC-compressed texture: ATC_INTERPOLATED_ALPHA
	ATC_INTERPOLATED_ALPHA,
	//! Default texture format: AUTO
	DEFAULT = AUTO,

	NONE = -1
};

typedef struct
{
	unsigned char*	_data;
	long			_dataLen;
	long			_width;
	long			_height;
	PixelFormat		_renderFormat;	
}jpg_data;


class textureJpegManager :
	public CSingleton<textureJpegManager>
{
public:
	textureJpegManager();
	~textureJpegManager();

	bool getJpgData(const char* filename, jpg_data& jpgData);

protected:
	bool initWithJpgData(const unsigned char * data, long dataLen, jpg_data& jpgData);
};

