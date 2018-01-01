#include "stdafx.h"
#include "BspTree.h"

bspPolygon::bspPolygon(const Vector3& pp1, const Vector3& pp2, const Vector3& pp3, void* userdata)
{
	p1 = pp1;
	p2 = pp2;
	p3 = pp3;
	userData = userdata;

	Vector3::cross(p2 - p1, p3 - p1, normal);
	if (normal.z < 0.0)
	{
		normal.x = -normal.x;
		normal.y = -normal.y;
		normal.z = -normal.z;
	}
	normal.normalize();

	distance = -Vector3::dot(normal, p1);
}

bool bspPolygon::classifyPoint(Vector3 point)
{
	float dot = Vector3::dot(normal, point);

	float dis = dot - distance;
	if (dis <= FLT_EPSILON)
		return bspPolygon::COINCIDING;
	else if (dis > 0.0f)
		return bspPolygon::INFRONT;
	else
		return bspPolygon::BEHIND;
}

//反之未必成立
bool bspPolygon::classifyPoly(const bspPolygon& poly)
{
	int s1 = classifyPoint(poly.p1);
	int s2 = classifyPoint(poly.p2);
	int s3 = classifyPoint(poly.p3);

	return (s1 == s2 && s2 == s3) ? s1 : bspPolygon::SPANNING;
}

//交点p, 线段起点a1,a2, 方向d，p1p2，平面法线n，点p3
//method1：可得p=a1+td, (p1-p)*(p2-p)=normal，p带入，求t
//method2：求(a2-a1)与n的点积和(p3-a1)与a1点积的比，乘以d，求p
bool bspPolygon::intersectLine(const LineSegment& line, Vector3& interPoint)
{
	float dotp = Vector3::dot(line.dir, normal);
	float norp = Vector3::dot((p1 - line.p1), normal);
	float rp = norp / dotp;
	Vector3 interP = line.p1 + line.dir*rp;

}

//所有的单个面都在其它面之前或重合
bool bspPolygon::isContexPolygons(const std::vector<bspPolygon*>& polygons)
{
	for (int p1 = 0; p1 < polygons.size(); ++p1)
	{
		for (int p2 = 0; p2 < polygons.size(); ++p2)
		{
			if (p1 != p2)
			{
				int relation = polygons[p1]->classifyPoly(*polygons[p2]);
				if (relation == bspPolygon::BEHIND)
					return false;
			}
		}
	}
	return true;
}

BspTree::BspTree()
{

}

void BspTree::generateTree(const std::vector<bspPolygon*>& polygons, bspNode* nd)
{
	bspPolygon* dividePoly = choiceBestDividePolygon(polygons);
	if (nullptr == nd)
	{
		m_rootNode = new bspNode();
		nd = m_rootNode;
	}
	nd->isLeaf = nullptr == dividePoly;
	nd->polygons = polygons;
	if (nd->isLeaf)
	{
		return;
	}
	nd->front = new bspNode();
	nd->behind = new bspNode();
	nd->dividers.push_back(dividePoly);

	std::vector<bspPolygon*> frontPolygons, behindPolygons;
	for (int n = 0; n < polygons.size(); ++n)
	{
		if (polygons[n] != dividePoly)
		{
			switch (dividePoly->classifyPoly(*polygons[n]))
			{
			case bspPolygon::INFRONT: {frontPolygons.push_back(polygons[n]); }break;
			case bspPolygon::BEHIND: {behindPolygons.push_back(polygons[n]); }break;
			case bspPolygon::SPANNING: {nd->spans.push_back(polygons[n]); }break;
			case bspPolygon::COINCIDING: {nd->dividers.push_back(polygons[n]); }break;
			}
		}
	}
	generateTree(behindPolygons, nd->behind);
	generateTree(frontPolygons, nd->front);
}

bspPolygon* BspTree::choiceBestDividePolygon(const std::vector<bspPolygon*>& polygons)
{
	float leastRelation = 0.2f;
	float bestRelation = 0.2f;
	int leastSplits = 9999;
	const float MIN_RELATION_SCALE = 0.9f;
	bspPolygon* bestPoly = nullptr;
	int polysCnt = polygons.size();

	if (polysCnt == 0)
		goto GOTO_BREAK;
	bestPoly = polygons[1];

	if (bspPolygon::isContexPolygons(polygons))
	{
		bestPoly = nullptr;
		goto GOTO_BREAK;
	}
	if (polysCnt == 1)
		goto GOTO_BREAK;
		
	
	for (int p1 = 0; p1 != polygons.size(); ++p1)
	{
		int frontNum = 0;
		int behindNum = 0;
		int coincideNum = 0;
		int spanNum = 0;

		float relation = 0.0f;
		for (int p2 = 0; p2 != polygons.size(); ++p2)
		{
			if (p1 != p2)
			{
				switch (polygons[p1]->classifyPoly(*polygons[p2]))
				{
					case bspPolygon::INFRONT: {frontNum++; }break;
					case bspPolygon::BEHIND: {behindNum++; }break;
					case bspPolygon::COINCIDING: {coincideNum++; }break;
					case bspPolygon::SPANNING: {spanNum++; }break;
				}
			}

			relation = frontNum > behindNum ? (float)behindNum / (float)frontNum : (float)frontNum / (float)behindNum;

			if (relation > leastRelation &&
				(spanNum < leastSplits || (spanNum == leastSplits && relation > bestRelation))
				)
			{
				bestPoly = polygons[p1];
				leastSplits = spanNum;
				bestRelation = relation;
				leastRelation /= MIN_RELATION_SCALE;
				if (leastRelation > 1.0f)
				{
					goto GOTO_BREAK;
				}
			}
		}
	}

GOTO_BREAK:
	return bestPoly;
}

bool BspTree::isConvexSet(const std::vector<bspPolygon*>& polygons)
{
	return true;
}
