#pragma once
#include <vector>
#include "scene.h"
#include "GL\glew.h"
#include "Vector2.h"
#include "tech\billboardTechnique.h"
#include "Node.h"
#include "Pipeline.h"

class BillboardList :public Node
{
public:
	BillboardList();
	~BillboardList();

	

	void setPosition(float x, float y, float z);

	void setScale(float s);

	void update(float ft);
	void draw();
	
protected:
	void init();

protected:
	billboardTechnique* m_tech = nullptr;
	int m_texture;
	Pipeline m_pipe;
	GLuint VBO;
};
