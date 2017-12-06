#pragma once
#include "Vector3.h"
#include "Plane.h"

class Line
{
public:
	Line();
	Line(const Vector3f& p1, const Vector3f& p2);
	bool intersectLine(const Line& plane);


public:
	Vector3f p1;
	Vector3f p2;
};



