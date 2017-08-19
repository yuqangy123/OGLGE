#include "stdafx.h"
#include <math.h>
#include "stdio.h"
#include "textureJpegManager.h"
extern "C" {  
	#include "jpeglib.h" 
}

/*
 * 当libjpeg-turbo为vs2010编译时，vs2015下静态链接libjpeg-turbo会链接出错:找不到__iob_func,
 * 增加__iob_func到__acrt_iob_func的转换函数解决此问题,
 * 当libjpeg-turbo用vs2015编译时，不需要此补丁文件
 */
#if _MSC_VER>=1900
#include "stdio.h" 
_ACRTIMP_ALT FILE* __cdecl __acrt_iob_func(unsigned); 
#ifdef __cplusplus 
extern "C" 
#endif 
FILE* __cdecl __iob_func(unsigned i) { 
    return __acrt_iob_func(i); 
}
#endif /* _MSC_VER>=1900 */

typedef struct MyErrorMgr * MyErrorPtr;
METHODDEF(void)
myErrorExit(j_common_ptr cinfo)
{
	/* cinfo->err really points to a MyErrorMgr struct, so coerce pointer */
	MyErrorPtr myerr = (MyErrorPtr)cinfo->err;

	/* Always display the message. */
	/* We could postpone this until after returning, if we chose. */
	/* internal message function can't show error message in some platforms, so we rewrite it here.
	* edit it if has version conflict.
	*/
	//(*cinfo->err->output_message) (cinfo);
	char buffer[JMSG_LENGTH_MAX];
	(*cinfo->err->format_message) (cinfo, buffer);
	printf("jpeg error: %s", buffer);

}

typedef int jmp_buf[16];
struct MyErrorMgr
{
	struct jpeg_error_mgr pub;  /* "public" fields */
	jmp_buf setjmp_buffer;  /* for return to caller */
};
DECLARE_SINGLETON_MEMBER(textureJpegManager);
textureJpegManager::textureJpegManager()
{
}


textureJpegManager::~textureJpegManager()
{
}

bool textureJpegManager::getJpgData(const char* filename, jpg_data& jpgData)
{
	FILE* fp = nullptr;
	fopen_s(&fp, filename, "r");
	if (nullptr == fp)
		return false;
	
	long length = 0;
	fseek(fp, 0, SEEK_END);
	length = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if (length == 0)
		return false;

	unsigned char* buf = new unsigned char[length + 1];
	buf[0] = 0;

	if (0 != fread(buf, length, 1, fp))
		return false;

	fclose(fp);
	fp = nullptr;

	return initWithJpgData(buf, length, jpgData);
}

bool textureJpegManager::initWithJpgData(const unsigned char * data, long dataLen, jpg_data& jpgData)
{
	/* these are standard libjpeg structures for reading(decompression) */
	struct jpeg_decompress_struct cinfo;
	/* We use our private extension JPEG error handler.
	* Note that this struct must live as long as the main JPEG parameter
	* struct, to avoid dangling-pointer problems.
	*/
	struct MyErrorMgr jerr;
	/* libjpeg data structure for storing one row, that is, scanline of an image */
	JSAMPROW row_pointer[1] = { 0 };
	unsigned long location = 0;

	bool ret = false;
	do
	{
		/* We set up the normal JPEG error routines, then override error_exit. */
		cinfo.err = jpeg_std_error(&jerr.pub);
		jerr.pub.error_exit = myErrorExit;
		

		/* setup decompression process and source, then read JPEG header */
		jpeg_create_decompress(&cinfo);

#ifndef CC_TARGET_QT5
		jpeg_mem_src(&cinfo, const_cast<unsigned char*>(data), dataLen);
#endif /* CC_TARGET_QT5 */

		/* reading the image header which contains image information */
#if (JPEG_LIB_VERSION >= 90)
		// libjpeg 0.9 adds stricter types.
		jpeg_read_header(&cinfo, TRUE);
#else
		jpeg_read_header(&cinfo, TRUE);
#endif

		// we only support RGB or grayscale
		if (cinfo.jpeg_color_space == JCS_GRAYSCALE)
		{
			jpgData._renderFormat = PixelFormat::I8;
		}
		else
		{
			cinfo.out_color_space = JCS_RGB;
			jpgData._renderFormat = PixelFormat::RGB888;
		}

		/* Start decompression jpeg here */
		jpeg_start_decompress(&cinfo);
		cinfo.output_height = 416;
		/* init image info */
		jpgData._width = cinfo.output_width;
		jpgData._height = cinfo.output_height;

		long _dataLen = cinfo.output_width*cinfo.output_height*cinfo.output_components;
		unsigned char* _data = new unsigned char[_dataLen * sizeof(unsigned char)];
		
		if (nullptr == _data)
			break;

		/* now actually read the jpeg into the raw buffer */
		/* read one scan line at a time */
		while (cinfo.output_scanline < cinfo.output_height)
		{
			row_pointer[0] = _data + location;
			location += cinfo.output_width*cinfo.output_components;
			jpeg_read_scanlines(&cinfo, row_pointer, 1);
		}

		/* When read image file with broken data, jpeg_finish_decompress() may cause error.
		* Besides, jpeg_destroy_decompress() shall deallocate and release all memory associated
		* with the decompression object.
		* So it doesn't need to call jpeg_finish_decompress().
		*/
		//jpeg_finish_decompress( &cinfo );
		jpeg_destroy_decompress(&cinfo);
		/* wrap up decompression, destroy objects, free pointers and close open files */
		ret = true;

		jpgData._dataLen = _dataLen + ceil(_dataLen / 3);
		jpgData._data = new unsigned char[jpgData._dataLen * sizeof(unsigned char)];
		unsigned char* tdata = jpgData._data;
		
		//convert to RGBA8888
		for (int n = 0; n < _dataLen-2; n+=3)
		{
			*tdata++ = *(_data + n + 0);
			*tdata++ = *(_data + n + 1);
			*tdata++ = *(_data + n + 2);
			*tdata++ = 0xFF;
		}
		
	} while (0);
	
	
	return ret;

}