#pragma once
#include "Node.h"
#include <vector>

class CScene : public Node
{
public:
	CScene();
	~CScene();

	void init();
	void addNode(Node* nd);
	void update(float ft);
	void draw();
	void keyInput(unsigned char param, int x, int y);
	void mouseInput(int button, int state, int x, int y);

protected:
	std::vector<Node*> m_nodes;
};

