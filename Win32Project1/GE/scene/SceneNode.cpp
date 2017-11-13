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

void SceneNode::addNode(Node* nd, uint prior)
{
	if (nullptr == nd)
	{
		printf("%s sceneNode addNode failed, node is null\r\n", m_nodeName);
		return;
	}
	
	m_nodes.add(nd, prior);
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
	int n = 0;
	for (auto itr = m_nodes.begin(); itr != m_nodes.end(); ++itr)
	{
		(*itr)->update(ft);
	}
}

void SceneNode::draw()
{
	for (auto itr = m_nodes.begin(); itr != m_nodes.end(); ++itr)
	{
		(*itr)->draw();
	}
}