#pragma once
#include <vector>
#include "scene.h"
#include "GL\glew.h"
#include "Vector2.h"
#include "tech\shadowMapTechnique.h"
#include "Node.h"
#include "Pipeline.h"
#include "MeshNode.h"
#include "Camera3D.h"

class meshShadowMap :public Node
{
public:
	meshShadowMap();
	~meshShadowMap();

	bool loadMesh(const char* filename);

	void setPosition(float x, float y, float z);

	void setScale(float s);

	void update(float ft);
	void draw();

	void keyInput(unsigned char param, int x, int y);

protected:
	void init();
	void initFBO();

	void drawMesh();
	void drawShadowMap();

protected:
	shadowMapTechnique* m_tech;
	
	Pipeline m_pipe;
	Matrix4f m_MVPMt4;
	GLuint VBO;
	GLuint IBO;

	MeshNode*	m_mesh;

	Pipeline m_lightPipe;
	Camera3D* m_pLightSpot;
	Matrix4f m_lightMVPMt4;

	GLuint m_fbo;
	GLuint m_shadowMap;
};
