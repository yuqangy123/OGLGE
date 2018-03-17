

#include "stdio.h"
#include "assert.h"
#include <math.h>
#include <string.h>
#include "TextureTgaLoader.h"


// These defines are used to tell us about the type of TARGA file it is
#define TGA_RGB         2        // This tells us it's a normal RGB (really BGR) file
#define TGA_A         3        // This tells us it's a ALPHA file
#define TGA_RLE        10        // This tells us that the targa is Run-Length Encoded (RLE)


#pragma pack( push, 1 )            //禁止字节自动对齐
typedef struct
{
	GLubyte        descLen;
	GLubyte        cmapType;
	GLubyte        imageType;
	GLshort        cmapStart;
	GLushort    cmapEntries;
	GLubyte        cmapBits;
	GLushort    xOffset;
	GLushort    yOffset;
}Header;

typedef struct
{
	Header        head;
	GLushort    width;
	GLushort    height;
	GLubyte        bpp;
	GLubyte        attrib;
}TGAHeader;


DECLARE_SINGLETON_MEMBER(TextureTgaLoader);
TextureTgaLoader::TextureTgaLoader()
{
}


TextureTgaLoader::~TextureTgaLoader()
{
}


bool TextureTgaLoader::getTgaData(const char* filename, img_tga_data& tgaData)
{
	if (filename == NULL)
		return false;
	/*
	0 - 文件中没有图像数据
		1 - 未压缩的，颜色表图像
		2 - 未压缩的，rgb 图像
		3 - 未压缩的，黑白图像
		9 - runlength 编码的颜色表图像
		10 - runlength 编码的 rgb 图像
		11 - 压缩的，黑白图像
		32 - 使用 huffman, delta 和 runlength 编码的颜色表图像
		33 - 使用 huffman, delta 和 runlength 编码的颜色映射图像，4
	*/
	Header uTGAcompare = { 0,0,2,0,0,0,0,0 };        //2为非压缩RGB格式        3  -  未压缩的，黑白图像
	Header cTGAcompare = { 0,0,10,0,0,0,0,0 };        //10为压缩RGB格式

	TGAHeader header;
	FILE* file = fopen(filename, "rb");
	if (!file) {
		printf("Openf file %s failed!\n", filename);
		return false;
	}

	if (fread(&header, 1, sizeof(TGAHeader), file) != sizeof(TGAHeader)) {        //读取TGA整个头结构体
		if (file)
			fclose(file);
		printf("Read data failed\n");
		return false;
	}

	tgaData.width = header.width;
	tgaData.height = header.height;
	tgaData.bpp = header.bpp;

	if (header.bpp == 32)
		tgaData.imageType = GL_RGBA;
	else if (header.bpp = 24)
		tgaData.imageType = GL_RGB;
	else {
		printf("Image type error!\n");
		return false;
	}

	uTGAcompare.cmapBits = header.head.cmapBits;
	if (memcmp(&uTGAcompare, &header.head, sizeof(header.head)) == 0) {            //未压缩TGA
		tgaData.bCompressed = false;
		if (!loadUncompressedTGA(&tgaData, file)) {
			printf("Load uncompressed TGA failed!\n");
			return false;
		}
	}
	else if (memcmp(&cTGAcompare, &header.head, sizeof(header.head)) == 0) {    //压缩TGA
		tgaData.bCompressed = true;
		if (!loadCompressedTGA(&tgaData, file)) {
			printf("Load compressed TGA failed!\n");
			return false;
		}
	}
	else {
		printf("Error TGA type!\n");
		return false;
	}

	return true;

}

bool TextureTgaLoader::loadUncompressedTGA(img_tga_data* texture, FILE* file)
{
	assert(file != NULL && texture != NULL);

	GLuint bytePerPixel = texture->bpp / 8;
	GLuint imgSize = texture->width*texture->height*bytePerPixel;                //图像总字节数
	texture->imageData = new GLubyte[imgSize];
	texture->dataLen = imgSize;

	if (fread(texture->imageData, 1, imgSize, file) != imgSize)
	{
		printf("Read texture imagedata failed!\n");
		return false;
	}

	//TGA采用了逆OpenGL的格式,要将BGR转换成为RGB
	// Go through all of the pixels and swap the B and R values since TGA
	// files are stored as BGR instead of RGB (or use GL_BGR_EXT verses GL_RGB)
	for (int i = 0; i < (int)imgSize; i += bytePerPixel) {
		/**//*GLushort temp = texture->imageData[i];
			texture->imageData[i] = texture->imageData[i+2];
			texture->imageData[i+2] = temp;*/
		texture->imageData[i] ^= texture->imageData[i + 2] ^= texture->imageData[i] ^= texture->imageData[i + 2];        //位操作提高速度,更换B,R分量
	}

	::fclose(file);
	return true;
}

bool TextureTgaLoader::loadCompressedTGA(img_tga_data* texture, FILE* file)
{
	assert(file != NULL && texture != NULL);

	GLuint bytePerPixel = texture->bpp / 8;
	GLuint imgSize = texture->width*texture->height*bytePerPixel;
	texture->imageData = new GLubyte[imgSize];
	texture->dataLen = imgSize;

	GLuint pixelcount = texture->width * texture->height;
	GLuint currentPixel = 0;        //当前正在读取的像素
	GLuint currentByte = 0;            //当前正在向图像中写入的像素
	GLubyte *colorbuffer = (GLubyte *)new GLubyte[bytePerPixel];    // 一个像素的存储空间s

	do
	{
		GLubyte chunkHeader = 0;        //存储ID块值的变量
		if (!fread(&chunkHeader, 1, sizeof(GLubyte), file)) {
			return false;
		}
		if (chunkHeader < 128)            //RAW块
		{
			chunkHeader++;                // 变量值加1以获取RAW像素的总数

			for (int i = 0; i < chunkHeader; i++) {
				if (fread(colorbuffer, 1, sizeof(bytePerPixel), file) != sizeof(bytePerPixel)) {
					printf("Read pixel failed!\n");
					return false;
				}
				texture->imageData[currentByte] = colorbuffer[2];
				texture->imageData[currentByte + 1] = colorbuffer[1];
				texture->imageData[currentByte + 2] = colorbuffer[0];
				if (bytePerPixel == 4)
					texture->imageData[currentByte + 3] = colorbuffer[3];

				currentPixel++;
				currentByte += bytePerPixel;
			}
		}
		else//下一段处理描述RLE段的“块”头。首先我们将chunkheader减去127来得到获取下一个颜色重复的次数。 
		{
			chunkHeader -= 127;            //减去127获得ID bit 的rid    开始循环拷贝我们多次读到内存中的像素，这由RLE头中的值规定。

			if (fread(colorbuffer, 1, sizeof(bytePerPixel), file) != sizeof(bytePerPixel)) {
				printf("Read pixel failed!\n");
				return false;
			}

			for (int i = 0; i < chunkHeader; i++) {
				texture->imageData[currentByte] = colorbuffer[2];                // 拷贝“R”字节
				texture->imageData[currentByte + 1] = colorbuffer[1];            // 拷贝“G”字节
				texture->imageData[currentByte + 2] = colorbuffer[0];            // 拷贝“B”字节
				if (bytePerPixel == 4)
					texture->imageData[currentByte + 3] = colorbuffer[3];            // 拷贝“A”字节

				currentPixel++;
				currentByte += bytePerPixel;
			}
		}
	} while (currentPixel < pixelcount);

	delete colorbuffer;
	::fclose(file);
	return true;
}
