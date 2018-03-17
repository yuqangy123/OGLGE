

#include "stdio.h"
#include "assert.h"
#include <math.h>
#include <string.h>
#include "TextureTgaLoader.h"


// These defines are used to tell us about the type of TARGA file it is
#define TGA_RGB         2        // This tells us it's a normal RGB (really BGR) file
#define TGA_A         3        // This tells us it's a ALPHA file
#define TGA_RLE        10        // This tells us that the targa is Run-Length Encoded (RLE)


#pragma pack( push, 1 )            //��ֹ�ֽ��Զ�����
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
	0 - �ļ���û��ͼ������
		1 - δѹ���ģ���ɫ��ͼ��
		2 - δѹ���ģ�rgb ͼ��
		3 - δѹ���ģ��ڰ�ͼ��
		9 - runlength �������ɫ��ͼ��
		10 - runlength ����� rgb ͼ��
		11 - ѹ���ģ��ڰ�ͼ��
		32 - ʹ�� huffman, delta �� runlength �������ɫ��ͼ��
		33 - ʹ�� huffman, delta �� runlength �������ɫӳ��ͼ��4
	*/
	Header uTGAcompare = { 0,0,2,0,0,0,0,0 };        //2Ϊ��ѹ��RGB��ʽ        3  -  δѹ���ģ��ڰ�ͼ��
	Header cTGAcompare = { 0,0,10,0,0,0,0,0 };        //10Ϊѹ��RGB��ʽ

	TGAHeader header;
	FILE* file = fopen(filename, "rb");
	if (!file) {
		printf("Openf file %s failed!\n", filename);
		return false;
	}

	if (fread(&header, 1, sizeof(TGAHeader), file) != sizeof(TGAHeader)) {        //��ȡTGA����ͷ�ṹ��
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
	if (memcmp(&uTGAcompare, &header.head, sizeof(header.head)) == 0) {            //δѹ��TGA
		tgaData.bCompressed = false;
		if (!loadUncompressedTGA(&tgaData, file)) {
			printf("Load uncompressed TGA failed!\n");
			return false;
		}
	}
	else if (memcmp(&cTGAcompare, &header.head, sizeof(header.head)) == 0) {    //ѹ��TGA
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
	GLuint imgSize = texture->width*texture->height*bytePerPixel;                //ͼ�����ֽ���
	texture->imageData = new GLubyte[imgSize];
	texture->dataLen = imgSize;

	if (fread(texture->imageData, 1, imgSize, file) != imgSize)
	{
		printf("Read texture imagedata failed!\n");
		return false;
	}

	//TGA��������OpenGL�ĸ�ʽ,Ҫ��BGRת����ΪRGB
	// Go through all of the pixels and swap the B and R values since TGA
	// files are stored as BGR instead of RGB (or use GL_BGR_EXT verses GL_RGB)
	for (int i = 0; i < (int)imgSize; i += bytePerPixel) {
		/**//*GLushort temp = texture->imageData[i];
			texture->imageData[i] = texture->imageData[i+2];
			texture->imageData[i+2] = temp;*/
		texture->imageData[i] ^= texture->imageData[i + 2] ^= texture->imageData[i] ^= texture->imageData[i + 2];        //λ��������ٶ�,����B,R����
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
	GLuint currentPixel = 0;        //��ǰ���ڶ�ȡ������
	GLuint currentByte = 0;            //��ǰ������ͼ����д�������
	GLubyte *colorbuffer = (GLubyte *)new GLubyte[bytePerPixel];    // һ�����صĴ洢�ռ�s

	do
	{
		GLubyte chunkHeader = 0;        //�洢ID��ֵ�ı���
		if (!fread(&chunkHeader, 1, sizeof(GLubyte), file)) {
			return false;
		}
		if (chunkHeader < 128)            //RAW��
		{
			chunkHeader++;                // ����ֵ��1�Ի�ȡRAW���ص�����

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
		else//��һ�δ�������RLE�εġ��顱ͷ���������ǽ�chunkheader��ȥ127���õ���ȡ��һ����ɫ�ظ��Ĵ����� 
		{
			chunkHeader -= 127;            //��ȥ127���ID bit ��rid    ��ʼѭ���������Ƕ�ζ����ڴ��е����أ�����RLEͷ�е�ֵ�涨��

			if (fread(colorbuffer, 1, sizeof(bytePerPixel), file) != sizeof(bytePerPixel)) {
				printf("Read pixel failed!\n");
				return false;
			}

			for (int i = 0; i < chunkHeader; i++) {
				texture->imageData[currentByte] = colorbuffer[2];                // ������R���ֽ�
				texture->imageData[currentByte + 1] = colorbuffer[1];            // ������G���ֽ�
				texture->imageData[currentByte + 2] = colorbuffer[0];            // ������B���ֽ�
				if (bytePerPixel == 4)
					texture->imageData[currentByte + 3] = colorbuffer[3];            // ������A���ֽ�

				currentPixel++;
				currentByte += bytePerPixel;
			}
		}
	} while (currentPixel < pixelcount);

	delete colorbuffer;
	::fclose(file);
	return true;
}
