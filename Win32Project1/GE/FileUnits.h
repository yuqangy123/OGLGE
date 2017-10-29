#pragma once
#include "Node.h"
#include <vector>
#include <string>
#include "Singleton.h"

#define FileUnitIns FileUnits::InstanceEx()
class FileUnits  : public CSingleton<FileUnits>
{
public:
	FileUnits();
	~FileUnits();

	void init();
	
	void getAbsoluteFilePath(const char* filepath, std::string& res);

	void getFileData(const char* file, std::string& res);

	bool writeData(const char* absoluteFile, const char* mode, const char* data, unsigned int dataLen);

protected:
	std::string m_workDir;
};

