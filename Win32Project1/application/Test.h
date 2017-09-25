#pragma once

#include "shaders.h"
#include <string>
#include "Node.h"
#include "tech/testTechnique.h"


class Test :public Node
{
public:
	Test();
	~Test();
	virtual void update(float ft) ;
	virtual void draw() ;
	virtual void keyInput(unsigned char param, int x, int y);
	virtual void mouseInput(int button, int state, int x, int y);
	void RenderSceneCB();

protected:
	void init();
	void CreateIndexBuffer();
	void CreateVertexBuffer();

protected:
	testTechnique* m_tech = nullptr;

	GLuint VBO;
	GLuint IBO;
};

