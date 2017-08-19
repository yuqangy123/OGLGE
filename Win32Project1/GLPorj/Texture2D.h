#pragma once
#include "Node.h"
#include <string>
#include "Pipeline.h"
#include "Texture2DRender.h"

class Texture2D : public Node
{
public:
	Texture2D(const std::string& resPath);
	~Texture2D();
	void update(float ft);
	void draw();

protected:
	void init();

protected:
	Pipeline m_pipe;

	std::string m_resourceFile;
	GLuint m_texturesID;
	V3F_T2F* m_renderdata = nullptr;
};

