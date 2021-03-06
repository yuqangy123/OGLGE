#pragma once
#include "Node.h"
#include "base.h"
#include "struct/llist.h"

#define SCENENODE_NAME_LENGTH 32

USING_NS_STRUCT;

class SceneNode : public Node
{
public:
	SceneNode();
	~SceneNode();

	bool init(const char* sceneNodeName = nullptr);

	void addNode(Node* nd, int prior=0);

	bool delNode(Node* nd);
	
	void update(float ft);

	void draw();

protected:
	llist<Node*> m_nodes;

	char m_nodeName[SCENENODE_NAME_LENGTH];
};

