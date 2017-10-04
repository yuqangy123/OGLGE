#include "stdafx.h"
#include "FileUnits.h"
#include "OGLGE.h"

#define MAX_PATH 512

DECLARE_SINGLETON_MEMBER(FileUnits)
FileUnits::FileUnits()
{
	init();
}


FileUnits::~FileUnits()
{

}

void FileUnits::init()
{
	char dir[MAX_PATH] = { 0 };

	GetCurrentDirectoryA(MAX_PATH, dir);
	m_workDir.assign(dir);
}

void FileUnits::getAbsoluteFilePath(const char* filepath, std::string& res)
{
	char dirszFilePath[MAX_PATH] = { 0 };

	sprintf(dirszFilePath, "%s/%s", m_workDir.c_str(), filepath);
	res.assign(dirszFilePath);
}

void FileUnits::getFileData(const char* file, std::string& res)
{
	FILE* fp = nullptr;
	fp = fopen(file, "r");
	if (nullptr == fp)
	{
		std::string fname;
		getAbsoluteFilePath(file, fname);
		fp = fopen(file, "r");
		if (nullptr == fp)
		{
			printf("file %s cant find", file);
			assert(0);
			return;
		}

		
	}
	long length = 0;
	fseek(fp, 0, SEEK_END);
	length = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	unsigned char* buf = new unsigned char[length + 1];
	memset(buf, 0x0, length + 1);

	auto readres = fread(buf, length, 1, fp);
	if (0 != readres && 1 != readres)
	{
		printf("file %s cant read", file);
		fclose(fp);
		assert(0);
		return;
	}
	
	res.assign((char*)buf);

	fclose(fp);
	fp = nullptr;
	delete buf;
}

bool FileUnits::writeData(const char* absoluteFile, const char* mode, const char* data, unsigned int dataLen)
{
	FILE* fp = nullptr;
	fp = fopen(absoluteFile, mode);
	if (nullptr == fp)
	{
		printf("writeData failed, file:%s\r\n", absoluteFile);
		return false;
	}

	fwrite(data, 1, dataLen, fp);

	fflush(fp);

	fclose(fp);
}