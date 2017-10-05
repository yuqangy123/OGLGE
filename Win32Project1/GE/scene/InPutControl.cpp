#include "stdafx.h"
#include "InPutControl.h"

DECLARE_SINGLETON_MEMBER(InPutControl)
InPutControl::InPutControl()
{
	init();
}


InPutControl::~InPutControl()
{

}

void InPutControl::init()
{
	
}


void InPutControl::keyInput(unsigned char param, int x, int y)
{
	for (int n = 0; n < m_nodes.size(); ++n)
	{
		m_nodes[n]->keyInput(param, x, y);
	}
}

void InPutControl::mouseInput(int button, int state, int x, int y)
{
	for (int n = 0; n < m_nodes.size(); ++n)
	{
		m_nodes[n]->mouseInput(button, state, x, y);
	}
}

void InPutControl::addListenNode(InPutInterface* nd)
{
	if (nullptr != nd)
	{
		m_nodes.push_back(nd);
	}
}
void InPutControl::removeListenNode(InPutInterface* nd)
{
	for (auto itr = m_nodes.begin(); itr != m_nodes.end(); ++itr)
	{
		if (*itr == nd)
		{
			m_nodes.erase(itr);
			break;
		}
	}
}