#pragma once
#include "Vector3.h"
#include "Plane.h"
#include "AABBBox.h"

class Ray
{
public:
	Ray();
	Ray(const Vector3f& originalPosition, const Vector3f& direction);
	bool intersectPlane(const Plane& plane, Vector3f& hitPosition);
	bool intersectAABBBox(const AABBBox& box, Vector3f& hitPosition);
	
public:
	Vector3f original;
	Vector3f dir;
	float t;
};



