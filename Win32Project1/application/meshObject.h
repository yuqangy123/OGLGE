#pragma once
#include <vector>
#include "scene.h"
#include "GL\glew.h"
#include "Vector2.h"
#include "tech\lightTechnique.h"
#include "Node.h"
#include "Pipeline.h"
#include "MeshNode.h"
#include "InPutInterface.h"

class meshObject :public Node, public InPutInterface
{
public:
	meshObject();
	~meshObject();

	bool loadMesh(const char* filename);

	void setPosition(float x, float y, float z);

	void setScale(float s);

	void update(float ft);
	void draw();

	void keyInput(unsigned char param, int x, int y);
	void mouseInput(int button, int state, int x, int y);
	
protected:
	void init();

protected:
	lightTechnique* m_tech;
	Matrix4f m_MVPMt4;
	Pipeline m_pipe;
	Vector3 m_ambientLightColor;
	float m_ambientLightIntensity;
	Vector3 m_diffuseDirection;
	float m_diffuseIntensity;

	MeshNode*	m_mesh;

	Vector4 m_world_point;
};
