#pragma once
#include "Node.h"
#include <vector>
#include <string>
#include "xFreeTypeFont.h"
#include "Singleton.h"

#define statsNodeInstance statisticalNode::InstanceEx()
#define ADD_DRAW_BATCHES_COUNT(times) statsNodeInstance.updateDrawnBatches(times)
#define ADD_DRAW_VERTICES_COUNT(times) statsNodeInstance.updateDrawnVertices(times)

class statisticalNode : public Node , public CSingleton<statisticalNode>
{
public:
	statisticalNode();
	~statisticalNode();

	void displayStats(bool b);

	void updateDrawnBatches(unsigned int cnt);
	void updateDrawnVertices(unsigned int cnt);


protected:
	void init();
	
protected:
	unsigned int m_drawnBatches;
	unsigned int m_drawnvetrices;

	uiSystem::xFreeTypeFont* m_drawBatchesLabel;
	uiSystem::xFreeTypeFont* m_drawVerticesLabel;

	bool m_display;
};

