#include "stdafx.h"
#include "SceneNode.h"



SceneNode::SceneNode()
{
	m_nodeName[0] = 0;
}


SceneNode::~SceneNode()
{

}

bool SceneNode::init(const char* sceneNodeName)
{
	DO
	if (nullptr != sceneNodeName)
	{
		if (strlen(sceneNodeName) < SCENENODE_NAME_LENGTH)
		{
			strcpy_s(m_nodeName, sceneNodeName);
		}
		else
		{
			printf("sceneNode init failed, name is too long, name:%s\r\n\r\n", sceneNodeName);
			IF_BREAK(false);
		}
	}
	WHILE

	return true;
}

void SceneNode::addNode(Node* nd)
{
	if (nullptr == nd)
	{
		printf("%s sceneNode addNode failed, node is null\r\n", m_nodeName);
		return;
	}
	
	m_nodes.push_back(nd);
	m_nodeses.push((unsigned int)nd, (unsigned int)nd);
}

bool SceneNode::delNode(Node* nd)
{
	for (auto itr = m_nodes.begin(); itr != m_nodes.end(); ++itr)
	{
		if (*itr == nd)
		{
			m_nodes.erase(itr);
			return true;
		}
	}
	return false;
}

void SceneNode::update(float ft)
{
	for (int n = 0; n < m_nodes.size(); ++n)
	{
		m_nodes[n]->update(ft);
	}
}

void SceneNode::draw()
{
	for (int n = 0; n < m_nodes.size(); ++n)
	{
		m_nodes[n]->draw();
	}
}