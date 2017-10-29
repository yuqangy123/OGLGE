#pragma once
#include "Node.h"
#include <string>
#include "Pipeline.h"


class freeTypeObject : public Node
{
public:
	freeTypeObject(const std::string& _str);
	~freeTypeObject();

	void setPosition(float x, float y);
	void setScale(float s);

	void update(float  ft);
	void draw();

protected:
	void init();

protected:
	Pipeline m_pipe;

	std::string m_resourceFile;
	unsigned int m_texturesID;
	std::string m_str;
	V3F_T2F* m_renderdata;
};

