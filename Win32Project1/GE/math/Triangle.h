#pragma once
#include "Vector3.h"


class Triangle
{
public:
	Triangle();
	Triangle(const Vector3f& pp1, const Vector3f& pp2, const Vector3f& pp3);

public:
	Vector3f p1;
	Vector3f p2;
	Vector3f p3;

	Vector3f normal;
	float distance;
};



