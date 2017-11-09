#include "stdafx.h"
#include "statisticalNode.h"
#include <windows.h> 
#include "SceneManager.h"

#define MAX_PATH 512

DECLARE_SINGLETON_MEMBER(statisticalNode)
statisticalNode::statisticalNode()
{
	init();
}


statisticalNode::~statisticalNode()
{
	displayStats(false);
}

void statisticalNode::init()
{
	m_drawBatchesLabel = nullptr;
	m_drawVerticesLabel = nullptr;
	m_display = false;
}

void statisticalNode::displayStats(bool b)
{
	if (m_display != b)
	{
		if (b)
		{
			m_drawBatchesLabel = newFreeFont;
			m_drawVerticesLabel = newFreeFont;

			DefaultSceneIns.addNode(m_drawBatchesLabel);
			DefaultSceneIns.addNode(m_drawVerticesLabel);
		}
		else
		{
			DefaultSceneIns.delNode(m_drawBatchesLabel);
			DefaultSceneIns.delNode(m_drawVerticesLabel);

			m_drawBatchesLabel = nullptr;
			m_drawVerticesLabel = nullptr;
		}
	}
}

