#pragma once
#include <vector>
#include "scene.h"
#include "GL\glew.h"
#include "Vector2.h"
#include "tech\skinnedMeshTechnique.h"
#include "Node.h"
#include "Pipeline.h"
#include "skinMeshNode.h"
#include "Camera3D.h"

class meshSkinnedAnimation :public Node
{
public:
	meshSkinnedAnimation();
	~meshSkinnedAnimation();

	bool loadMesh(const char* filename, const Vector3& pos, const Vector3& sal);


	void setScale(float s);

	void Rotate(float x=0.0f, float y = 0.0f, float z = 0.0f);

	void setPosition(float x, float y, float z);

	void update(float ft);
	void draw();

	void keyInput(unsigned char param, int x, int y);

protected:
	void init();


protected:
	skinnedMeshTechnique* m_tech;
	Pipeline m_pipe;
	Matrix4f m_WVPMt4;
	skinMeshNode* m_mesh=nullptr;

};