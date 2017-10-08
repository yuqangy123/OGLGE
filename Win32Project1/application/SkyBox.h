#pragma once
#include <vector>
#include "scene.h"
#include "GL\glew.h"
#include "Vector2.h"
#include "tech\skyBoxTechnique.h"
#include "Node.h"
#include "Pipeline.h"
#include "ModelMesh.h"

class SkyBox :public Node
{
public:
	SkyBox();
	~SkyBox();

	bool loadMesh(std::vector<std::string>& imgs);

	void setPosition(float x, float y, float z);

	void setScale(float s);

	void update(float ft);
	void draw();
	
protected:
	void init();

protected:
	skyBoxTechnique* m_tech;
	ModelMesh* m_mesh;
	int m_texture;
	Pipeline m_pipe;
};
