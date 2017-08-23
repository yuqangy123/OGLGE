#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "Mat4.h"
#include "Quaternion.h"
#include "shaders.h"
#include <string>
#include <map>
#include "Node.h"
#include "Texture2DRender.h"
#include "Pipeline.h"
#include "tech/lightTechnique.h"


class triangleVertebralLight :public Node
{
public:
	triangleVertebralLight();
	~triangleVertebralLight();
	virtual void update(float ft) ;
	virtual void draw() ;
	virtual void keyInput(unsigned char param, int x, int y);
	virtual void mouseInput(int button, int state, int x, int y);
	void RenderSceneCB();

protected:
	void init();

protected:
	lightTechnique* m_tech = nullptr;

	GLuint m_vbo[2];
	
	GLuint m_exturesID;
	


	Matrix4f m_worldMt4;
	Pipeline m_pipe;

	int m_lastMouseX;
	int m_lastMouseY;
};

