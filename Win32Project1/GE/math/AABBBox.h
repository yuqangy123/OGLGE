#pragma once
#include "Vector3.h"


class AABBBox
{
public:
	AABBBox();
	AABBBox(const Vector3f& minPosition, const Vector3f& maxPosition);

	
public:
	Vector3f min;
	Vector3f max;
};



