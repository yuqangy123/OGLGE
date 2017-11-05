#pragma once
#include "SceneNode.h"
#include <vector>

class Scene
{
public:
	Scene();
	~Scene();

	void init();
	void addNode(SceneNode* nd, int updateLevel = 0);
	void addNode(Node* nd, int updateLevel=0);
	void delNode(Node* nd);
	void update(float ft);
	void draw();
	

protected:
	std::vector<SceneNode*> m_nodes;
	SceneNode* m_default_scenenode;
};