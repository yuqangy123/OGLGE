#pragma once
#include "Vector3.h"
#include "Triangle.h"
#include <vector>
#include "renderer\common.h"

template<class T>
class bspPolygon
{
public:
	bspPolygon() {};
	bspPolygon(const T& pp1, const T& pp2, const T& pp3) { p1 = pp1; p2 = pp2; p3 = pp3; };
	virtual bool classifyPoint(Vector3 point)=0;
	virtual bool classifyPoly(bspPolygon* poly)=0;
	virtual void init()=0;

public:
	T p1;
	T p2;
	T p3;

	Vector3f normal;
	float distance;
};

template<class T>
 struct bspNode
{
	bspNode() :isLeaf(false), left(nullptr), right(nullptr), divider(nullptr){};

	bool isLeaf;
	bspNode* left;
	bspNode* right;

	bspPolygon<T>* divider;
	std::vector<bspPolygon<T>*> polygons;
};

template<class VectexT>
class BspTree
{
public:
	BspTree();
	bool generateTree(const std::vector<bspPolygon<VectexT>*>& polygons);
	


protected:
	bspPolygon<VectexT>* choiceBestDividePolygon(const std::vector<bspPolygon<VectexT>*>& polygons);
	bool isConvexSet(const std::vector<bspPolygon<VectexT>*>& polygons);
	

protected:
	bspNode<VectexT>* rootNode = nullptr;

};
