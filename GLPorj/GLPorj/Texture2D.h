#pragma once
#include "Node.h"
#include <string>

class Texture2D : public Node
{
public:
	Texture2D(const std::string& resPath);
	~Texture2D();


protected:
	std::string m_resourceFile;
};

