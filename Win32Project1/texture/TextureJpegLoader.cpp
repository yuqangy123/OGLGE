#include "stdafx.h"
#include <math.h>
#include <string.h>
#include "TextureJpegLoader.h"

#include "libjpeg/jpeglib.h"

// Define plugin and include the CImg Library.
#define cimg_plugin "jpeg_buffer.h"
#include "CImg.h"
#include "jpeg_buffer.h"
using namespace cimg_library;


/*
* 当libjpeg-turbo为vs2010编译时，vs2015下静态链接libjpeg-turbo会链接出错:找不到__iob_func,
* 增加__iob_func到__acrt_iob_func的转换函数解决此问题,
* 当libjpeg-turbo用vs2015编译时，不需要此补丁文件
*/
/*
#if _MSC_VER>=1900  
#include "stdio.h"   
_ACRTIMP_ALT FILE* __cdecl __acrt_iob_func(unsigned);
#ifdef __cplusplus   
extern "C"
#endif   
FILE* __cdecl __iob_func(unsigned i) {
	return __acrt_iob_func(i);
}
#endif 
*/


DECLARE_SINGLETON_MEMBER(TextureJpegLoader);
TextureJpegLoader::TextureJpegLoader()
{
}


TextureJpegLoader::~TextureJpegLoader()
{
}

int get_CImgRgbPixBufLen(CImg<unsigned char> & image)
{
	const unsigned int
		align = (4 - (3 * image._width) % 4) % 4,
		buf_size = (3 * image._width + align)*image.height();


	return buf_size;
}

unsigned char * get_CImgRgbPixBuf(CImg<unsigned char> & image, unsigned int& bufsize)
{
	const unsigned int
		align = (4 - (3 * image._width) % 4) % 4,
		buf_size = (3 * image._width + align)*image.height();

	bufsize = buf_size;
	unsigned char * pixRgb = new unsigned char[buf_size];
	unsigned char * pix = pixRgb;


	const unsigned char
		*ptr_r = image.data(0, image._height - 1, 0, 0),
		*ptr_g = (image._spectrum >= 2) ? image.data(0, image._height - 1, 0, 1) : 0,
		*ptr_b = (image._spectrum >= 3) ? image.data(0, image._height - 1, 0, 2) : 0;




	switch (image._spectrum)
	{
	case 1:
	{
	} break;
	case 2:
	{
	} break;
	default:
	{
		for (int ny = 0; ny < image._height; ny++)
		{
			for (int nx = 0; nx < image._width; nx++)
			{
				*pix++ = *ptr_b++;
				*pix++ = *ptr_g++;
				*pix++ = *ptr_r++;
			}


			pix += align;
			ptr_r -= 2 * image._width; ptr_g -= 2 * image._width; ptr_b -= 2 * image._width;
		}
	}
	}


	return pixRgb;


}
bool TextureJpegLoader::getJpgData(const char* filename, jpg_data& jpgData)
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
bool TextureJpegLoader::initWithJpgData(const unsigned char * data, long dataLen, jpg_data& jpgData)
{
	/* these are standard libjpeg structures for reading(decompression) */
	struct jpeg_decompress_struct cinfo;
	/* We use our private extension JPEG error handler.
	* Note that this struct must live as long as the main JPEG parameter
	* struct, to avoid dangling-pointer problems.
	*/
	//struct MyErrorMgr jerr;
	/* libjpeg data structure for storing one row, that is, scanline of an image */
	JSAMPROW row_pointer[1] = { 0 };
	unsigned long location = 0;

	bool ret = false;
	do
	{
		struct jpeg_decompress_struct cinfo;
		struct jpeg_error_mgr jerr;
		cinfo.err = jpeg_std_error(&jerr);
		
		/* We set up the normal JPEG error routines, then override error_exit. */
		//cinfo.err = jpeg_std_error(&jerr.pub);
		//jerr.pub.error_exit = myErrorExit;


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
		}
		else
		{
			cinfo.out_color_space = JCS_RGB;
		}

		/* Start decompression jpeg here */
		jpeg_start_decompress(&cinfo);
		/* init image info */
		jpgData._width = cinfo.output_width;
		jpgData._height = cinfo.output_height;

		long _dataLen = cinfo.output_width*cinfo.output_height*cinfo.output_components;
		unsigned char* _data = new unsigned char[_dataLen * sizeof(unsigned char)];

		if (nullptr == _data)
			break;

		/* now actually read the jpeg into the raw buffer */
		/* read one scan line at a time */
		int cnt = 0;
		while (cinfo.output_scanline < cinfo.output_height)
		{
			row_pointer[0] = _data + location;
			location += cinfo.output_width*cinfo.output_components;
			cnt = cnt + 1;
			printf("cnt:%d\r\n", cnt);
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

		int _dataLen_new = _dataLen + ceil(_dataLen / 3);
		jpgData._data = new unsigned char[_dataLen_new * sizeof(unsigned char)];
		unsigned char* tdata = jpgData._data;

		//convert to RGBA8888
		for (int n = 0; n < _dataLen - 2; n += 3)
		{
			*tdata++ = *(_data + n + 0);
			*tdata++ = *(_data + n + 1);
			*tdata++ = *(_data + n + 2);
			*tdata++ = 0xFF;
		}

		delete _data;

	} while (0);


	return ret;

}
bool TextureJpegLoader::getJpgDataEx(const char* filename, jpg_data& jpgData)
{
	std::FILE *file_input = std::fopen(filename, "rb");
	if (!file_input) { std::fprintf(stderr, "Input JPEG file not found !"); std::exit(0); }

	std::fprintf(stderr, " - Construct input JPEG-coded buffer\n");
	unsigned long buf_size = 1024*1024*10; // Put the file size here !
	JOCTET *buffer_input = new JOCTET[buf_size];
	if (std::fread(buffer_input, sizeof(JOCTET), buf_size, file_input)) std::fclose(file_input);
	// -> 'buffer_input' is now a valid jpeg-coded memory buffer.

	
	//////////////////////////////////////////
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_mem_src(&cinfo, const_cast<unsigned char*>(buffer_input), buf_size);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	const unsigned int row_stride = cinfo.output_width * cinfo.output_components;
	unsigned int _dataLen = cinfo.output_width*cinfo.output_height*cinfo.output_components;
	JOCTET *buf = new JOCTET[_dataLen];
	JSAMPROW row_pointer[1];
	while (cinfo.output_scanline < cinfo.output_height) {
		row_pointer[0] = buf + cinfo.output_scanline*row_stride;
		jpeg_read_scanlines(&cinfo, row_pointer, 1);
	}
	//jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	int _dataLen_new = _dataLen + ceil(_dataLen / 3);
	jpgData._data = new unsigned char[_dataLen_new * sizeof(unsigned char)];
	jpgData._width = cinfo.output_width;
	jpgData._height = cinfo.output_height;

	//convert to RGBA8888
	unsigned char* tdata = jpgData._data;
	JOCTET *tmpbuf = buf;
	for (int n = 0; n < _dataLen - 2; n += 3)
	{
		*tdata++ = *(tmpbuf + n + 0);
		*tdata++ = *(tmpbuf + n + 1);
		*tdata++ = *(tmpbuf + n + 2);
		*tdata++ = 0xFF;
	}
	delete buf;

	return true;
	//////////////////////////////////////////
	

	/*
	// Create a CImg instance from the jpeg-coded buffer using the plug-in function.
	std::fprintf(stderr, " - Create CImg instance from JPEG-coded buffer\n");
	CImg<unsigned char> img;
	img.load_jpeg_buffer(buffer_input, buf_size);
	delete[] buffer_input;



	unsigned int _dataLen = 0;
	unsigned char * pixels = get_CImgRgbPixBuf(img, _dataLen);
	
	int _dataLen_new = _dataLen + ceil(_dataLen / 3);
	jpgData._data = new unsigned char[_dataLen_new * sizeof(unsigned char)];
	jpgData._width = img._width;
	jpgData._height = img._height;

	//convert to RGBA8888
	unsigned char* tdata = jpgData._data;
	for (int n = 0; n < _dataLen - 2; n += 3)
	{
		*tdata++ = *(pixels + n + 0);
		*tdata++ = *(pixels + n + 1);
		*tdata++ = *(pixels + n + 2);
		*tdata++ = 0xFF;
	}
	delete pixels;
	*/
	return true;
}