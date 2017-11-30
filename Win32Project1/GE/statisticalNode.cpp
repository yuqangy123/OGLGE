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

	m_drawBatchesCount = m_drawVerticesCount = 0;
}

void statisticalNode::displayStats(bool b)
{
	if (m_display != b)
	{
		if (b)
		{
			m_drawBatchesLabel = newFreeFont;
			m_drawBatchesLabel->init();
			m_drawBatchesLabel->setPosition(0, 40);
			m_drawVerticesLabel = newFreeFont;
			m_drawVerticesLabel->init();
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

		m_display = b;
	}
}

void statisticalNode::updateDrawnBatches(unsigned int cnt)
{
	m_drawBatchesCount += cnt;
}
void statisticalNode::updateDrawnVertices(unsigned int cnt)
{
	m_drawVerticesCount += cnt;
}

void statisticalNode::clear()
{
	m_drawBatchesCount = m_drawVerticesCount = 0;
}

void statisticalNode::draw()
{
	if (m_display)
	{
		static wchar_t batchStr[64];
		static wchar_t verticeStr[64];
		swprintf_s(batchStr, L"draw batches:%d", m_drawBatchesCount);
		swprintf_s(batchStr, L"draw vertices:%d", m_drawVerticesCount);

		m_drawBatchesLabel->setString(batchStr);
		m_drawVerticesLabel->setString(verticeStr);
	}	
}