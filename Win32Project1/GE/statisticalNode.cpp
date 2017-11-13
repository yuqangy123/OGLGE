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
			m_drawBatchesLabel->setString(L"draw batch:0");
			m_drawBatchesLabel->setPosition(0, 100);
			m_drawVerticesLabel = newFreeFont;
			m_drawVerticesLabel->setString(L"draw vertices:0");
			m_drawVerticesLabel->setPosition(0, 30);

			DefaultSceneIns.addNode(m_drawBatchesLabel, -1);
			//DefaultSceneIns.addNode(m_drawVerticesLabel, -1);
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

