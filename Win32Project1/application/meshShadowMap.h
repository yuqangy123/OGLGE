#pragma once
#include <vector>
#include "scene.h"
#include "GL\glew.h"
#include "Vector2.h"
#include "tech\shadowMapMeshTechnique.h"
#include "Node.h"
#include "Pipeline.h"
#include "MeshNode.h"
#include "Camera3D.h"

class meshShadowMap :public Node
{
public:
	meshShadowMap();
	~meshShadowMap();

	bool loadMesh(const char* filename, const Vector3& pos, const Vector3& sal);

	void setTargetPosition(float x, float y, float z);
	void setLightPosition(float x, float y, float z);

	void setScale(float s);

	void setPosition(float x, float y, float z);

	void update(float ft);
	void draw();

	void keyInput(unsigned char param, int x, int y);

protected:
	void init();


protected:
	shadowMapMeshTechnique* m_tech;
	std::vector<MeshNode*> m_meshs;
	Pipeline m_pipe;
	Matrix4f m_MVPMt4;
	GLuint VBO;
	GLuint IBO;
};
