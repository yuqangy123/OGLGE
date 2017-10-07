#pragma once
#include <vector>
#include "scene.h"
#include "GL\glew.h"
#include "Vector2.h"
#include "tech\bumpNormalLightTechnique.h"
#include "bumpNormalMeshNode.h"
#include "Pipeline.h"
#include "MeshNode.h"

class meshbumpNormal :public Node
{
public:
	meshbumpNormal();
	~meshbumpNormal();

	bool loadMesh(const char* filename, const char* normalMapFile);

	void setPosition(float x, float y, float z);

	void setScale(float s);

	void update(float ft);
	void draw();

	void keyInput(unsigned char param, int x, int y);
	
protected:
	void init();

protected:
	bumpNormalLightTechnique* m_tech;
	Pipeline m_pipe;

	Vector3 m_ambientLightColor;
	float m_ambientLightIntensity;
	float m_diffuseIntensity;

	Vector3 m_lightPosition;
	float m_weakContant;
	float m_weakLine;

	bumpNormalMeshNode*	m_mesh;
	GLuint VBO;
	GLuint IBO;
	unsigned int m_tex;
	unsigned int m_normalTex;
	float switchNormal;
};
