#pragma once
#include <vector>
#include "scene.h"
#include "GL\glew.h"
#include "Vector2.h"
#include "tech\shadowMapMeshTechnique.h"
#include "tech\shadowMeshTechnique.h"
#include "Node.h"
#include "Pipeline.h"
#include "MeshNode.h"
#include "Camera3D.h"

class meshShadow :public Node
{
public:
	meshShadow();
	~meshShadow();

	bool loadMesh(const char* filename, const Vector3& pos, const Vector3& sal);

	void setTargetPosition(float x, float y, float z);
	void setLightPosition(float x, float y, float z);

	void update(float ft);
	void draw();

	void keyInput(unsigned char param, int x, int y);

protected:
	void init();


protected:
	MeshNode* m_mesh = nullptr;
	MeshNode* m_terrainQuadMesh = nullptr;
	shadowMapMeshTechnique* m_shadowMapTech=nullptr;
	shadowMeshTechnique* m_tech = nullptr;
	
	std::vector<MeshNode*> m_meshs;

	Pipeline m_pipe;
	Matrix4f m_MVPMt4;

	Vector3 m_ambientLightColor;
	float m_ambientLightIntensity;
	Vector3 m_diffuseDirection;
	float m_diffuseIntensity;
};
