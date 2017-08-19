#include "stdafx.h"
#include "Scene.h"
#include "OGLGE.h"


CScene::CScene()
{
}


CScene::~CScene()
{

}

void CScene::init()
{
}

void CScene::addNode(Node* nd)
{
	if (nullptr != nd)
	{
		m_nodes.push_back(nd);
	}
}

void CScene::update(float ft)
{
	for (auto itr = m_nodes.begin(); itr != m_nodes.end(); ++itr)
	{
		(*itr)->update(ft);
	}
}

void CScene::draw()
{
	for (auto itr = m_nodes.begin(); itr != m_nodes.end(); ++itr)
	{
		(*itr)->draw();
	}
}

void CScene::keyInput(unsigned char param, int x, int y)
{
	for (auto itr = m_nodes.begin(); itr != m_nodes.end(); ++itr)
	{
		(*itr)->keyInput(param, x, y);
	}
}

void CScene::mouseInput(int button, int state, int x, int y)
{
	for (auto itr = m_nodes.begin(); itr != m_nodes.end(); ++itr)
	{
		(*itr)->mouseInput(button, state, x, y);
	}
}