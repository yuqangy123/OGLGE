#pragma once
#include "Node.h"
#include <vector>
#include <string>
#include "Singleton.h"

class FileUnits  : public CSingleton<FileUnits>
{
public:
	FileUnits();
	~FileUnits();

	void init();
	
	void getAbsoluteFilePath(const char* filepath, std::string& res);

	void getFileData(const char* file, std::string& res);

protected:
	std::string m_workDir;
};

