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
#include "tech/normalTexTechnique.h"


class JikeTest :public Node
{
public:
	JikeTest();
	~JikeTest();
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
	normalTexTechnique* m_tech = nullptr;

	GLuint VBO;
	GLuint IBO;

	GLuint m_lastTexturesID;
	


	Matrix4f m_MVPMt4;
	Pipeline m_pipe;

	int m_lastMouseX;
	int m_lastMouseY;
};

