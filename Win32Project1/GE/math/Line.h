#pragma once
#include "Vector3.h"
#include "Plane.h"

class Line
{
public:
	Line();
	Line(const Vector3f& p1, const Vector3f& p2);

	//coplaner test
	bool coplane(const Line& plane);
	bool intersectLine(const Line& line);

public:
	Vector3f p1;
	Vector3f p2;
	Vector3f dir;
};



