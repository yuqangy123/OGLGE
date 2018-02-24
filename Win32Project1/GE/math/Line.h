#pragma once
#include "Vector3.h"
#include "Plane.h"

class LineSegment
{
public:
	LineSegment();
	LineSegment(const Vector3f& p1, const Vector3f& p2);

	//coplaner test
	bool coplane(const LineSegment& plane);

	bool intersectLine(const LineSegment& line, Vector3& interPoint);

	bool intersectPoint(const Vector3& point);

	Vector3 getPointAt(float yt);

public:
	Vector3f p1;
	Vector3f p2;
	Vector3f dir;
	float t;
};



