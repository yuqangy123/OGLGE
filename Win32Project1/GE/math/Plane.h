#pragma once
#include "Vector3.h"


class Plane
{
public:
	Plane();
	Plane(const Vector3f& n, const Vector3f& dotPosition);
	
public:
	Vector3f normal;
	Vector3f p;

};



