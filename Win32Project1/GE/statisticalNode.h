#pragma once
#include "Node.h"
#include <vector>
#include <string>
#include "xFreeTypeFont.h"
#include "Singleton.h"

#define statsNodeIns statisticalNode::InstanceEx()
#define ADD_DRAW_BATCHES_COUNT(times) statsNodeIns.updateDrawnBatches(times)
#define ADD_DRAW_VERTICES_COUNT(times) statsNodeIns.updateDrawnVertices(times)
#define ADD_DRAW_BATCHES_VERTICES_COUNT(batches, vertices) {statsNodeIns.updateDrawnBatches(batches);statsNodeIns.updateDrawnVertices(vertices);}

class statisticalNode : public Node , public CSingleton<statisticalNode>
{
public:
	statisticalNode();
	~statisticalNode();

	void displayStats(bool b);

	void updateDrawnBatches(unsigned int cnt);
	void updateDrawnVertices(unsigned int cnt);

	void clear();
	void draw();

protected:
	void init();
	
protected:
	unsigned int m_drawnBatches;
	unsigned int m_drawnvetrices;

	uiSystem::xFreeTypeFont* m_drawBatchesLabel;
	uiSystem::xFreeTypeFont* m_drawVerticesLabel;

	bool m_display;
	int m_drawBatchesCount;
	int m_drawVerticesCount;
};

