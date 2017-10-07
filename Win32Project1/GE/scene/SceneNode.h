#pragma once
#include "Node.h"
#include <vector>

#define SCENENODE_NAME_LENGTH 32

class SceneNode : public Node
{
public:
	SceneNode();
	~SceneNode();

	bool init(const char* sceneNodeName = nullptr);

	void addNode(Node* nd);

	bool delNode(Node* nd);
	
	void update(float ft);

	void draw();

protected:
	std::vector<Node*> m_nodes;
	char m_nodeName[SCENENODE_NAME_LENGTH];
};

