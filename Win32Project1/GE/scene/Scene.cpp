#include "stdafx.h"
#include "Scene.h"
#include "GL/glew.h"


Scene::Scene()
{
	init();
}


Scene::~Scene()
{

}

void Scene::init()
{
	m_default_scenenode = new SceneNode();
	m_default_scenenode->init("scene_default_scenenode");
	addNode(m_default_scenenode);
}

void Scene::addNode(SceneNode* nd, int updateLevel)
{
	if (nullptr != nd)
	{
		m_nodes.push_back(nd);
	}
}

void Scene::addNode(Node* nd, int updateLevel)
{
	if (nullptr != nd)
	{
		m_default_scenenode->addNode(nd, updateLevel);
	}
}

void Scene::delNode(Node* nd)
{
	for (int n = 0; n < m_nodes.size(); ++n)
	{
		if (m_nodes[n]->delNode(nd))
			break;
	}
}

void Scene::update(float ft)
{
	for (int n = 0; n < m_nodes.size(); ++n)
	{
		m_nodes[n]->update(ft);
	}
}

void Scene::draw()
{
	//开启透明度混合
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int n = 0; n < m_nodes.size(); ++n)
	{
		m_nodes[n]->draw();
	}

	glDisable(GL_BLEND);
}

