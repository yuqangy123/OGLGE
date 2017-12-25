#include "stdafx.h"
#include "statisticalNode.h"
#include <windows.h> 
#include "SceneManager.h"
#include <winsock.h>

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

	
	//获取每秒多少CPU Performance Tick 
	QueryPerformanceFrequency(&m_perfFreq);

	QueryPerformanceCounter(&m_perNow);
	m_perLast = m_perNow;
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

int gettimeofday(struct timeval * val, struct timezone *)
{
	if (val)
	{
		LARGE_INTEGER liTime, liFreq;
		QueryPerformanceFrequency(&liFreq);
		QueryPerformanceCounter(&liTime);
		val->tv_sec = (long)(liTime.QuadPart / liFreq.QuadPart);
		val->tv_usec = (long)(liTime.QuadPart * 1000000.0 / liFreq.QuadPart - val->tv_sec * 1000000.0);
	}
	return 0;
}


float _deltaTime;
struct timeval *_lastUpdate = new (std::nothrow) struct timeval;
void statisticalNode::update()
{
	m_drawBatchesCount = m_drawVerticesCount = 0;

	struct timeval now;

	if (gettimeofday(&now, nullptr) == 0)
	{
		_deltaTime = (now.tv_sec - _lastUpdate->tv_sec) + (now.tv_usec - _lastUpdate->tv_usec) / 1000000.0f;
		printf("_deltaTime=%f\r\n", _deltaTime);
		*_lastUpdate = now;
	}
	QueryPerformanceCounter(&m_perNow);
	LONGLONG t = (LONGLONG((m_perNow.QuadPart - m_perLast.QuadPart)*1000) / m_perfFreq.QuadPart);
	
	printf("t=%d\r\n", t);
	m_perLast = m_perNow;
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