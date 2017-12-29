#pragma once
#include "Vector3.h"
#include "Triangle.h"
#include <vector>
#include "renderer\common.h"


class bspPolygon
{
public:
	const static int COINCIDING	= 0;
	const static int BEHIND		= -1;
	const static int INFRONT	= 1;
	const static int SPANNING	= 2;

public:
	bspPolygon() {};
	bspPolygon(const Vector3& pp1, const Vector3& pp2, const Vector3& pp3, void* userdata=nullptr);
	bool classifyPoint(Vector3 point);
	bool classifyPoly(const bspPolygon& poly);
	static bool isContexPolygons(const std::vector<bspPolygon*>& polygons);

public:
	Vector3 p1;
	Vector3 p2;
	Vector3 p3;

	Vector3f normal;
	float distance;

	void* userData;
};

 struct bspNode
{
	bspNode() :isLeaf(false), front(nullptr), behind(nullptr){};

	bool isLeaf;
	bspNode* front;
	bspNode* behind;

	std::vector<bspPolygon*> dividers;
	std::vector<bspPolygon*> spans;
	std::vector<bspPolygon*> polygons;//还有一种方式就是叶结点保存绘制多边形
};

class BspTree
{
public:
	BspTree();
	void generateTree(const std::vector<bspPolygon*>& polygons, bspNode* nd = nullptr);
	

protected:
	bspPolygon* choiceBestDividePolygon(const std::vector<bspPolygon*>& polygons);
	bool isConvexSet(const std::vector<bspPolygon*>& polygons);
	

protected:
	bspNode* m_rootNode = nullptr;

};
